<?php


// Takes raw data from the request
$json = file_get_contents('php://input');

// Converts it into a PHP object
$data = json_decode($json);

$service = new MobicineService();

//$service->accessService("getSimilarDiseases", "ACANTHOSIS");  // result: ACANTHOSIS NIGRICANS
//$service->accessService("getSimilarSymptoms", "acanthosis");  //result: Benign acanthosis nigricans
//$service->accessService("getCodesFromSymptom", "Thick hyperpigmented flexural area skin"); //result: 100600
//$service->accessService("getCodesFromDisease", "ACANTHOSIS NIGRICANS"); //result: 100600
//$service->accessService("getSymptomsFromCode", 100600); //result: Benign acanthosis nigricans, Thick hyperpigmented flexural area skin
$service->accessService("getDiseasesFromCode", 100600); //result: ACANTHOSIS NIGRICANS

// $query = $data->query;

class MobicineService
{


    private $symptom_table = "symptoms";
    private $disease_table = "diseases";
    private $symptom_column = "symptom";
    private $disease_column = "disease";
    private $code_column = "code";
    private $max_line  = 100;


    function accessService($FunctionName, $Params)
    {
        $result = array();

        switch($FunctionName)
        {
            case 'getSimilarDiseases';

                $result = $this->getSimilarDiseases($Params);

                break;
            case 'getSimilarSymptoms';

                $result = $this->getSimilarSymptoms($Params);

                break;
            case 'getCodesFromSymptom';

                $result = $this->getCodesFromSymptom($Params);

                break;
            case 'getCodesFromDisease';

                $result = $this->getCodesFromDisease($Params);

                break;
            case 'getSymptomsFromCode';

                $result = $this->getSymptomsFromCode($Params);

                break;

            case 'getDiseasesFromCode';

                $result = $this->getDiseasesFromCode($Params);

                break;

            case 'getDifferentialSymptoms';

                $result = $this->getDifferentialSymptoms($Params);

                break;
            case 'getSameSymptoms';

                $result = $this->getSameSymptoms($Params);

                break;
            case 'getDifferentialDiseases';

                $result = $this->getDifferentialDiseases($Params);

                break;
            case 'getSameDiseases';

                $result = $this->getSameDiseases($Params);

                break;


            default;

                echo "Oppss... Something went wrong!";

                break;
        }

        $this->printResult($result);

    }



    function printResult($Result)
    {
        $line_count = 0;

        foreach ($Result as &$value)
        {
            echo $value."<br>";
            $line_count++;

            if($line_count > $this->max_line)
                break;
        }
    }



    function getSearchResult($Qry, $Column)
    {
        $result_list = array();

        $db = new DataBase();

        $db->open();

        $result_array = $db->sendQuery($Qry);
        if ($result_array->num_rows > 0)
        {
            // output data of each row
            while($row = $result_array->fetch_assoc())
            {
                array_push($result_list, utf8_encode(trim($row[$Column])));

            }
        }

        $db->close();

        return $result_list;
    }



    function getSimilarSymptoms($SymptomHint)
    {
        $qry = "SELECT ".$this->symptom_column." FROM ".$this->symptom_table." WHERE ".$this->symptom_column." LIKE '%".$SymptomHint."%'";

        return $this->getSearchResult($qry, $this->symptom_column);
    }



    function getSimilarDiseases($DiseaseHint)
    {
        $qry = "SELECT ".$this->disease_column." FROM ".$this->disease_table." WHERE ".$this->disease_column." LIKE '%".$DiseaseHint."%'";

        return $this->getSearchResult($qry, $this->disease_column);
    }



    function getCodesFromSymptom($Symptom)
    {
        $result_list = array();

        $qry = "SELECT ".$this->code_column." FROM ".$this->symptom_table." WHERE ".$this->symptom_column." LIKE '%".$Symptom."%'";

        $arr = $this->getSearchResult($qry, $this->code_column);
        $uniq_arr = array_unique($arr);

        return $uniq_arr;

    }



    function getCodesFromDisease($Disease)
    {
        $result_list = array();

        $qry = "SELECT ".$this->code_column." FROM ".$this->disease_table." WHERE ".$this->disease_column." LIKE '%".$Disease."%'";

        $arr = $this->getSearchResult($qry, $this->code_column);
        $uniq_arr = array_unique($arr);

        return $uniq_arr;
    }



    function getSymptomsFromCode($Code)
    {

        $result_list = array();

        $qry = "SELECT ".$this->symptom_column." FROM ".$this->symptom_table." WHERE ".$this->code_column." = ".$Code;

        $arr = $this->getSearchResult($qry, $this->symptom_column);
        $uniq_arr = array_unique($arr);

        return $uniq_arr;

    }



    function getDiseasesFromCode($Code)
    {
        $result_list = array();

        $qry = "SELECT ".$this->disease_column." FROM ".$this->disease_table." WHERE ".$this->code_column." = ".$Code;

        $arr = $this->getSearchResult($qry, $this->disease_column);
        $uniq_arr = array_unique($arr);

        return $uniq_arr;
    }



    function getDifferentialSymptoms($DiseaseList)
    {
        $result_list = array();
        $all_symptom_list = array();

        foreach ($DiseaseList as &$Disease)
        {
            $disease_code_list = $this->getCodesFromDisease($Disease);

            foreach ($disease_code_list as &$code)
            {
                $symptom_list = $this->getSymptomsFromCode($code);

                array_merge($all_symptom_list, $symptom_list);
            }
        }

        $withoutDuplicates = array_unique($all_symptom_list);

        return $withoutDuplicates;

    }



    function getSameSymptoms($DiseaseList)
    {
        $result_list = array();
        $all_symptom_list = array();

        foreach ($DiseaseList as &$Disease)
        {
            $disease_code_list = $this->getCodesFromDisease($Disease);

            foreach ($disease_code_list as &$code)
            {
                $symptom_list = $this->getSymptomsFromCode($code);

                array_merge($all_symptom_list, $symptom_list);
            }
        }

        $withoutDuplicates = array_unique($all_symptom_list);
        $duplicates = array_diff($all_symptom_list, $withoutDuplicates);
        $uniq_dublicates = array_unique($duplicates);

        return $uniq_dublicates;

    }



    function getDifferentialDiseases($SymptomList)
    {

        $result_list = array();
        $all_disease_list = array();

        foreach ($SymptomList as &$symptom)
        {
            $symptom_code_list = $this->getCodesFromSymptom($symptom);

            foreach ($symptom_code_list as &$code)
            {
                $diseases_list = $this->getDiseasesFromCode($code);

                array_merge($all_disease_list, $diseases_list);
            }
        }

        $withoutDuplicates = array_unique($all_disease_list);

        return $withoutDuplicates;

    }



    function getSameDiseases($SymptomList)
    {
        $result_list = array();
        $all_disease_list = array();

        foreach ($SymptomList as &$symptom)
        {
            $symptom_code_list = $this->getCodesFromSymptom($symptom);

            foreach ($symptom_code_list as &$code)
            {
                $diseases_list = $this->getDiseasesFromCode($code);

                array_merge($all_disease_list, $diseases_list);
            }
        }

        $withoutDuplicates = array_unique($all_disease_list);
        $duplicates = array_diff($all_disease_list, $withoutDuplicates);
        $uniq_dublicates = array_unique($duplicates);

        return $uniq_dublicates;

    }


}



class DataBase
{
  private $servername = "******";
  private $database = "******";
  private $username = "******";
  private $password = "*****";
  private $conn;

  function open()
  {
      // Create connection
      $this->conn = mysqli_connect($this->servername, $this->username, $this->password, $this->database);
      // Check connection
      if (!($this->conn))
      {
          die(" Connection error". "<br>");
          mysqli_connect_error();
      }


  }

  function sendQuery($Query)
  {
    $result = $this->conn->query($Query);
    return $result;
  }


  function close()
  {
    $this->conn->close();
  }

}


?>
