
#set the path to the main project and to the CPP testing and autoTester
$solution_path = "C:\Users\Christian\Documents\Lessons\NUS\Semester5\CS3201\SPA\cs3201\GeneralTesting\";

#all the other paths are relative to the $solution_path
$cppunit_relative_path = "UnitTesting\";
$integration_relative_path = "IntegrationTesting\";
$autoTester_relative_path = "AutoTester\";

$exe_cppunit = "Debug\UnitTesting.exe";
$exe_integration = "Debug\IntegrationTesting.exe";
$exe_autoTester = "Debug\AutoTester.exe";
$exe_autoTester1 = "Debug\AutoTester.exe";
#$exe_autoTester2 = "Debug\AutoTester.exe";
#$exe_autoTester3 = "Debug\AutoTester.exe";

#output filenames
$out_cppunit = $cpp_relative_path +"cppunit.out";
$out_integration = $integration_relative_path +"integration.out";
$out_autoTester = $autoTester_relative_path +"autoTester.out";
$out_autoTester1 = $autoTester_relative_path +"autoTester1.out";
#$out_autoTester2 = $autoTester_relative_path +"autoTester2.out";
#$out_autoTester3 = $autoTester_relative_path +"autoTester3.out";

#command line arguments for each executable
$args_cppunit = @(" ");
$args_integration = @($integration_relative_path);
$args_autoTester = @($autoTester_relative_path + "tests\simple00.txt " + $autoTester_relative_path + "tests\query00.txt " + $autoTester_relative_path + "tests\out00.xml");
$args_autoTester1 = @($autoTester_relative_path + "tests\simple01.txt " + $autoTester_relative_path + "tests\query01.txt " + $autoTester_relative_path + "tests\out01.xml");
#$args_autoTester2 = @($autoTester_relative_path + "tests\simple02.txt " + $autoTester_relative_path + "tests\query02.txt " + $autoTester_relative_path + "tests\out02.xml");
#$args_autoTester3 = @($autoTester_relative_path + "tests\simple03.txt " + $autoTester_relative_path + "tests\query03.txt " + $autoTester_relative_path + "tests\out03.xml");

#change directory to the $solution_path
$current_path = get-location;
chdir($solution_path);

$p = get-location;
write-output $p;

#tests that will be run
#add/remove items if you want to do only unit testing or autoTester testing
$all_testing = @("cppunit", "integration", "autoTester", "autoTester1"); #, "autoTester2", "autoTester3");

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

