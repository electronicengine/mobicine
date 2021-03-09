<?php

//$json = "{\n    \"FunctionName\": \"getPossibleDiseases\",\n    \"params\": [\n        \"Failure to thrive\"     ]\n}\n";
$json = file_get_contents('php://input');
$data = json_decode($json);

$function_name = $data->{"FunctionName"};
$params = $data->{"params"};

$service = new MobicineService();
$service->accessService($function_name, $params); //result: ACANTHOSIS NIGRICANS



//$service->accessService("getSimilarDiseases", "ACANTHOSIS");  // result: ACANTHOSIS NIGRICANS
//$service->accessService("getSimilarSymptoms", "Abdominal aortic aneurysm");  //result: Benign acanthosis nigricans
//$service->accessService("getCodesFromSymptom", "Thick hyperpigmented flexural area skin"); //result: 100600
//$service->accessService("getCodeFromDisease", "ACANTHOSIS NIGRICANS"); //result: 100600
//$service->accessService("getSymptomsFromCode", 100600); //result: Benign acanthosis nigricans, Thick hyperpigmented flexural area skin
//$service->accessService("getDiseaseFromCode", 100600); //result: ACANTHOSIS NIGRICANS

//$diseases = array("AORTIC ANEURYSM, FAMILIAL ABDOMINAL, 1; AAA1", "AORTIC ANEURYSM, FAMILIAL THORACIC 8; AAT8");
//$symptoms = array("Failure to thrive","Seizure", "Hyperreflexia"); 
//$service->accessService("getSameSymptoms", $diseases);
//$service->accessService("getDifferentialSymptoms", $diseases);
//$service->accessService("getPossibleDiseases", $symptoms);
//$service->accessService("getDifferentialDiseases", $symptoms);



class MobicineService
{


    private $symptom_table = "symptoms";
    private $disease_table = "diseases";
    private $symptom_column = "symptom";
    private $disease_column = "disease";
    private $code_column = "code";
    private $max_line  = 600;


    function accessService($FunctionName, $Params)
    {
        $result = array();

        switch($FunctionName)
        {
            case 'getSimilarDiseases';
				
                $result = $this->getSimilarDiseases($Params[0]);

                break;
            case 'getSimilarSymptoms';

                $result = $this->getSimilarSymptoms($Params[0]);

                break;
            case 'getCodesFromSymptom';

                $result = $this->getCodesFromSymptom($Params[0]);

                break;
            case 'getCodeFromDisease';

                $result = $this->getCodeFromDisease($Params[0]);

                break;
            case 'getSymptomsFromCode';

                $result = $this->getSymptomsFromCode($Params[0]);

                break;

            case 'getDiseaseFromCode';

                $result = $this->getDiseaseFromCode($Params[0]);

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
            case 'getPossibleDiseases';

                $result = $this->getPossibleDiseases($Params);

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

        $qry = "SELECT ".$this->code_column." FROM ".$this->symptom_table." WHERE ".$this->symptom_column." LIKE '".$Symptom."%'";

        $arr = $this->getSearchResult($qry, $this->code_column);
        $uniq_arr = array_unique($arr);

        return $uniq_arr;

    }



    function getCodeFromDisease($Disease)
    {
        $result_list = array();

        $qry = "SELECT ".$this->code_column." FROM ".$this->disease_table." WHERE ".$this->disease_column." LIKE '".$Disease."'";

        $arr = $this->getSearchResult($qry, $this->code_column);
		
        return $arr[0];
    }



    function getSymptomsFromCode($Code)
    {
        $result_list = array();

        $qry = "SELECT ".$this->symptom_column." FROM ".$this->symptom_table." WHERE ".$this->code_column." = ".$Code;

        $arr = $this->getSearchResult($qry, $this->symptom_column);
        $uniq_arr = array_unique($arr);

        return $uniq_arr;
    }



    function getDiseaseFromCode($Code)
    {
        $result_list = array();

        $qry = "SELECT ".$this->disease_column." FROM ".$this->disease_table." WHERE ".$this->code_column." = ".$Code;

        $arr = $this->getSearchResult($qry, $this->disease_column);

        return $arr[0];
    }



    function getDifferentialSymptoms($DiseaseList)
    {
		$all_symptoms = array();

        foreach ($DiseaseList as &$Disease)
        {
            $disease_code = $this->getCodeFromDisease($Disease);
			$symptoms = $this->getSymptomsFromCode($disease_code);

			$this->merge($all_symptoms, $symptoms);
        }
				
		$disease_count = sizeof($DiseaseList);

		if($disease_count == 1)
			return $all_symptoms;
		
		$repeated_symptoms= $this->extractRepeatedValues($all_symptoms ,$disease_count);
		$uniq_symptoms = array_unique($all_symptoms);
		$result = array_diff_assoc($uniq_symptoms, $repeated_symptoms);

		return $result;
    }



    function getSameSymptoms($DiseaseList)
    {
		
		$all_symptoms = array();

		foreach ($DiseaseList as &$Disease)
        {
			
   			$disease_code = $this->getCodeFromDisease($Disease);
			$symptoms = $this->getSymptomsFromCode($disease_code);

			$this->merge($all_symptoms, $symptoms);
			
        }

		$disease_count = sizeof($DiseaseList);

		$repeated_symptoms= $this->extractRepeatedValues($all_symptoms ,$disease_count);

		return $repeated_symptoms;

    }



    function getDifferentialDiseases($SymptomList)
    {

        $result_list = array();
        $all_disease_list = array();
		$all_code_list = array();
		$result_list = array();

        foreach ($SymptomList as &$symptom)
        {
            $symptom_code_list = $this->getCodesFromSymptom($symptom);
			$this->merge($all_code_list, $symptom_code_list);

        }

		$symptom_count = sizeof($SymptomList);
		
		$repeated_codes = $this->extractRepeatedValues($all_code_list ,$symptom_count);
		$uniq_codes = array_diff_assoc($all_code_list, $repeated_codes);
		
		foreach ($uniq_codes as &$code)
		{
			$disease = $this->getDiseaseFromCode($code);
			$result_list[] = $disease;
		}

		return $result_list;
		

    }



    function getPossibleDiseases($SymptomList)
    {
        $result_list = array();
        $all_disease_list = array();
		$all_code_list = array();
		$result_list = array();
		
        foreach ($SymptomList as &$symptom)
        {
            $symptom_code_list = $this->getCodesFromSymptom($symptom);

			$this->merge($all_code_list, $symptom_code_list);
        }

		$symptom_count = sizeof($SymptomList);

		$repeated_codes = $this->extractRepeatedValues($all_code_list ,$symptom_count);

		foreach ($repeated_codes as $code)
		{
			$disease = $this->getDiseaseFromCode($code);
			$result_list[] = $disease;
		}

		return $result_list;
    }
	
	
	
	function merge(&$array1, &$array2) 
	{
		foreach($array2 as $i) 
		{
			$array1[] = $i;
		}
	}
	
	
	
	function extractRepeatedValues(&$Array, $Count) 
	{				
		$counts = array_count_values($Array);
		$Count--;

		$repeated_title  = array_filter($Array, function ($value) use ($counts, $Count) {
			return $counts[$value] > $Count;
		});
            		
  		$repeated_title = array_unique($repeated_title);

		return $repeated_title;
  
	}
}




class DataBase
{
  private $servername = "";
  private $database = "";
  private $username = "";
  private $password = "";
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
