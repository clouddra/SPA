
#set the path to the main project and to the CPP testing and autoTester
$solution_path = ".\";

#all the other paths are relative to the $solution_path
$cppunit_relative_path = "UnitTesting\";
$integration_relative_path = "IntegrationTesting\";
$autoTester_relative_path = "AutoTester\";

$exe_cppunit = "Release\UnitTesting.exe";
$exe_integration = "Release\IntegrationTesting.exe";
$exe_autoTester = "Release\AutoTester.exe";
$exe_autoTester1 = "Release\AutoTester.exe";
$exe_autoTester2 = "Release\AutoTester.exe";
$exe_autoTester3 = "Release\AutoTester.exe";
$exe_autoTester4 = "Release\AutoTester.exe";
$exe_autoTester5 = "Release\AutoTester.exe";
$exe_autoTester6 = "Release\AutoTester.exe";
$exe_autoTester7 = "Release\AutoTester.exe";
$exe_autoTester8 = "Release\AutoTester.exe";

#output filenames
$out_cppunit = $cpp_relative_path +"cppunit.out";
$out_integration = $integration_relative_path +"integration.out";
$out_autoTester = $autoTester_relative_path +"autoTester.out";
$out_autoTester1 = $autoTester_relative_path +"autoTester1_simon.out";
$out_autoTester2 = $autoTester_relative_path +"autoTester2_simon.out";
$out_autoTester3 = $autoTester_relative_path +"autoTester3_simon.out";
$out_autoTester4 = $autoTester_relative_path +"autoTester4_simon.out";
$out_autoTester5 = $autoTester_relative_path +"autoTester5_simon.out";
$out_autoTester6 = $autoTester_relative_path +"autoTester6_simon.out";
$out_autoTester7 = $autoTester_relative_path +"autoTester7_simon.out";
$out_autoTester8 = $autoTester_relative_path +"autoTester8_simon.out";


#command line arguments for each executable
$args_cppunit = @(" ");
$args_integration = @($integration_relative_path);
$args_autoTester = @($autoTester_relative_path + "tests\SourceA.txt " + $autoTester_relative_path + "tests\QueriesA.txt " + $autoTester_relative_path + "tests\simonA.xml");
$args_autoTester1 = @($autoTester_relative_path + "tests\Source6.txt " + $autoTester_relative_path + "tests\Queries6.txt " + $autoTester_relative_path + "tests\simon6.xml");
$args_autoTester2 = @($autoTester_relative_path + "tests\Source8.txt " + $autoTester_relative_path + "tests\Queries8.txt " + $autoTester_relative_path + "tests\simon8.xml");
$args_autoTester3 = @($autoTester_relative_path + "tests\Source9.txt " + $autoTester_relative_path + "tests\Queries9.1.txt " + $autoTester_relative_path + "tests\simon9_1.xml");
$args_autoTester4 = @($autoTester_relative_path + "tests\Source10.txt " + $autoTester_relative_path + "tests\Queries10.txt " + $autoTester_relative_path + "tests\simon10.xml");
$args_autoTester5 = @($autoTester_relative_path + "tests\Source11.txt " + $autoTester_relative_path + "tests\Queries11.txt " + $autoTester_relative_path + "tests\simon11.xml");
$args_autoTester6 = @($autoTester_relative_path + "tests\Source9.txt " + $autoTester_relative_path + "tests\Queries9.2.txt " + $autoTester_relative_path + "tests\simon9_2.xml");
$args_autoTester7 = @($autoTester_relative_path + "tests\Source12.txt " + $autoTester_relative_path + "tests\Queries12.1.txt " + $autoTester_relative_path + "tests\simon12_1.xml");
$args_autoTester8 = @($autoTester_relative_path + "tests\Source12.txt " + $autoTester_relative_path + "tests\Queries12.2.txt " + $autoTester_relative_path + "tests\simon12_2.xml");

#change directory to the $solution_path
$current_path = get-location;
chdir($solution_path);

$p = get-location;
write-output $p;

#tests that will be run
#add/remove items if you want to do only unit testing or autoTester testing
#"autoTester11", "autoTester12", "autoTester13" -> contain recursion
#"cppunit", "integration", "autoTester", "autoTester1", "autoTester2", "autoTester3", "autoTester4", "autoTester5", "autoTester6", "autoTester7", "autoTester8", "autoTester9", "autoTester10", "autoTester14", "autoTester15", "autoTester16", "autoTester17", "autoTester18", "autoTester19", "autoTester20", "autoTester21", "autoTester22", "autoTester23", "autoTester24", "autoTester25", "autoTester26", "autoTester27"
#$all_testing = @("autotester", "autoTester1", "autoTester2", "autoTester3", "autoTester4", "autoTester5", "autoTester6", "autoTester7", "autoTester8");
$all_testing = @("autoTester1", "autoTester2", "autoTester3", "autoTester4", "autoTester5", "autoTester6");


foreach ($testing in $all_testing)
{
	$file = get-variable ("out_$testing") -valueOnly;
	$exe = get-variable ("exe_$testing") -valueOnly;
	$cmd_args = get-variable ("args_$testing") -valueOnly;
	
	foreach ($arg in $cmd_args)
	{
		$run_cmd = $solution_path+$exe+" " +$cmd_args+" > "+$file;
		$run_arg = $cmd_args+" > "+$file;

		write-output "Running: $run_cmd `n";
		invoke-expression $run_cmd;

		write-output "Output: `n---`n";

		get-content $file;
		
		write-output "---------`n";
		write-output "Done running $testing`n`n`n";
	}
}

#return to the initial path (where the script is)
chdir($current_path);

