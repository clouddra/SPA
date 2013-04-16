
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
$exe_autoTester9 = "Release\AutoTester.exe";
$exe_autoTester10 = "Release\AutoTester.exe";
$exe_autoTester11 = "Release\AutoTester.exe";
$exe_autoTester12 = "Release\AutoTester.exe";
$exe_autoTester13 = "Release\AutoTester.exe";
$exe_autoTester14 = "Release\AutoTester.exe";
$exe_autoTester15 = "Release\AutoTester.exe";
$exe_autoTester16 = "Release\AutoTester.exe";
$exe_autoTester17 = "Release\AutoTester.exe";
$exe_autoTester18 = "Release\AutoTester.exe";
$exe_autoTester19 = "Release\AutoTester.exe";
$exe_autoTester20 = "Release\AutoTester.exe";
$exe_autoTester21 = "Release\AutoTester.exe";
$exe_autoTester22 = "Release\AutoTester.exe";
$exe_autoTester23 = "Release\AutoTester.exe";
$exe_autoTester24 = "Release\AutoTester.exe";
$exe_autoTester25 = "Release\AutoTester.exe";
$exe_autoTester26 = "Release\AutoTester.exe";
$exe_autoTester27 = "Release\AutoTester.exe";
$exe_autoTester28 = "Release\AutoTester.exe";
$exe_autoTester29 = "Release\AutoTester.exe";
$exe_autoTester30 = "Release\AutoTester.exe";
$exe_autoTester31 = "Release\AutoTester.exe";
$exe_autoTester32 = "Release\AutoTester.exe";
$exe_autoTester33 = "Release\AutoTester.exe";
$exe_autoTester34 = "Release\AutoTester.exe";

#output filenames
$out_cppunit = $cpp_relative_path +"cppunit.out";
$out_integration = $integration_relative_path +"integration.out";
$out_autoTester = $autoTester_relative_path +"autoTester.out";
$out_autoTester1 = $autoTester_relative_path +"autoTester1.out";
$out_autoTester2 = $autoTester_relative_path +"autoTester2.out";
$out_autoTester3 = $autoTester_relative_path +"autoTester3.out";
$out_autoTester4 = $autoTester_relative_path +"autoTester4.out";
$out_autoTester5 = $autoTester_relative_path +"autoTester5.out";
$out_autoTester6 = $autoTester_relative_path +"autoTester6.out";
$out_autoTester7 = $autoTester_relative_path +"autoTester7.out";
$out_autoTester8 = $autoTester_relative_path +"autoTester8.out";
$out_autoTester9 = $autoTester_relative_path +"autoTester9.out";
$out_autoTester10 = $autoTester_relative_path +"autoTester10.out";
$out_autoTester11 = $autoTester_relative_path +"autoTester11.out";
$out_autoTester12 = $autoTester_relative_path +"autoTester12.out";
$out_autoTester13 = $autoTester_relative_path +"autoTester13.out";
$out_autoTester14 = $autoTester_relative_path +"autoTester14.out";
$out_autoTester15 = $autoTester_relative_path +"autoTester15.out";
$out_autoTester16 = $autoTester_relative_path +"autoTester16.out";
$out_autoTester17 = $autoTester_relative_path +"autoTester17.out";
$out_autoTester18 = $autoTester_relative_path +"autoTester18.out";
$out_autoTester19 = $autoTester_relative_path +"autoTester19.out";
$out_autoTester20 = $autoTester_relative_path +"autoTester20.out";
$out_autoTester21 = $autoTester_relative_path +"autoTester21.out";
$out_autoTester22 = $autoTester_relative_path +"autoTester22.out";
$out_autoTester23 = $autoTester_relative_path +"autoTester23.out";
$out_autoTester24 = $autoTester_relative_path +"autoTester24.out";
$out_autoTester25 = $autoTester_relative_path +"autoTester25.out";
$out_autoTester26 = $autoTester_relative_path +"autoTester26.out";
$out_autoTester27 = $autoTester_relative_path +"autoTester27.out";
$out_autoTester28 = $autoTester_relative_path +"autoTester28.out";
$out_autoTester29 = $autoTester_relative_path +"autoTester29.out";
$out_autoTester30 = $autoTester_relative_path +"autoTester30.out";
$out_autoTester31 = $autoTester_relative_path +"autoTester31.out";
$out_autoTester32 = $autoTester_relative_path +"autoTester32.out";
$out_autoTester33 = $autoTester_relative_path +"autoTester33.out";
$out_autoTester34 = $autoTester_relative_path +"autoTester34.out";

#command line arguments for each executable
$args_cppunit = @(" ");
$args_integration = @($integration_relative_path);
$args_autoTester = @($autoTester_relative_path + "tests\simple00.txt " + $autoTester_relative_path + "tests\query00.txt " + $autoTester_relative_path + "tests\out00.xml");
$args_autoTester1 = @($autoTester_relative_path + "tests\simple01.txt " + $autoTester_relative_path + "tests\query01.txt " + $autoTester_relative_path + "tests\out01.xml");
$args_autoTester2 = @($autoTester_relative_path + "tests\simple02.txt " + $autoTester_relative_path + "tests\query02.txt " + $autoTester_relative_path + "tests\out02.xml");
$args_autoTester3 = @($autoTester_relative_path + "tests\simple03.txt " + $autoTester_relative_path + "tests\query03.txt " + $autoTester_relative_path + "tests\out03.xml");
$args_autoTester4 = @($autoTester_relative_path + "tests\simple04.txt " + $autoTester_relative_path + "tests\query04.txt " + $autoTester_relative_path + "tests\out04.xml");
$args_autoTester5 = @($autoTester_relative_path + "tests\simple05.txt " + $autoTester_relative_path + "tests\query05.txt " + $autoTester_relative_path + "tests\out05.xml");
$args_autoTester6 = @($autoTester_relative_path + "tests\simple06.txt " + $autoTester_relative_path + "tests\query06.txt " + $autoTester_relative_path + "tests\out06.xml");
$args_autoTester7 = @($autoTester_relative_path + "tests\simple07.txt " + $autoTester_relative_path + "tests\query07.txt " + $autoTester_relative_path + "tests\out07.xml");
$args_autoTester8 = @($autoTester_relative_path + "tests\simple08.txt " + $autoTester_relative_path + "tests\query08.txt " + $autoTester_relative_path + "tests\out08.xml");
$args_autoTester9 = @($autoTester_relative_path + "tests\simple09.txt " + $autoTester_relative_path + "tests\query09.txt " + $autoTester_relative_path + "tests\out09.xml");
$args_autoTester10 = @($autoTester_relative_path + "tests\simple10.txt " + $autoTester_relative_path + "tests\query10.txt " + $autoTester_relative_path + "tests\out10.xml");
$args_autoTester11 = @($autoTester_relative_path + "tests\simple11.txt " + $autoTester_relative_path + "tests\query11.txt " + $autoTester_relative_path + "tests\out11.xml");
$args_autoTester12 = @($autoTester_relative_path + "tests\simple12.txt " + $autoTester_relative_path + "tests\query12.txt " + $autoTester_relative_path + "tests\out12.xml");
$args_autoTester13 = @($autoTester_relative_path + "tests\simple13.txt " + $autoTester_relative_path + "tests\query13.txt " + $autoTester_relative_path + "tests\out13.xml");
$args_autoTester14 = @($autoTester_relative_path + "tests\simple14.txt " + $autoTester_relative_path + "tests\query14.txt " + $autoTester_relative_path + "tests\out14.xml");
$args_autoTester15 = @($autoTester_relative_path + "tests\simple15.txt " + $autoTester_relative_path + "tests\query15.txt " + $autoTester_relative_path + "tests\out15.xml");
$args_autoTester16 = @($autoTester_relative_path + "tests\simple16.txt " + $autoTester_relative_path + "tests\query16.txt " + $autoTester_relative_path + "tests\out16.xml");
$args_autoTester17 = @($autoTester_relative_path + "tests\simple17.txt " + $autoTester_relative_path + "tests\query17.txt " + $autoTester_relative_path + "tests\out17.xml");
$args_autoTester18 = @($autoTester_relative_path + "tests\simple18.txt " + $autoTester_relative_path + "tests\query18.txt " + $autoTester_relative_path + "tests\out18.xml");
$args_autoTester19 = @($autoTester_relative_path + "tests\simple19.txt " + $autoTester_relative_path + "tests\query19.txt " + $autoTester_relative_path + "tests\out19.xml");
$args_autoTester20 = @($autoTester_relative_path + "tests\simple20.txt " + $autoTester_relative_path + "tests\query20.txt " + $autoTester_relative_path + "tests\out20.xml");
$args_autoTester21 = @($autoTester_relative_path + "tests\simple21.txt " + $autoTester_relative_path + "tests\query21.txt " + $autoTester_relative_path + "tests\out21.xml");
$args_autoTester22 = @($autoTester_relative_path + "tests\simple22.txt " + $autoTester_relative_path + "tests\query22.txt " + $autoTester_relative_path + "tests\out22.xml");
$args_autoTester23 = @($autoTester_relative_path + "tests\simple23.txt " + $autoTester_relative_path + "tests\query23.txt " + $autoTester_relative_path + "tests\out23.xml");
$args_autoTester24 = @($autoTester_relative_path + "tests\simple24.txt " + $autoTester_relative_path + "tests\query24.txt " + $autoTester_relative_path + "tests\out24.xml");
$args_autoTester25 = @($autoTester_relative_path + "tests\simple25.txt " + $autoTester_relative_path + "tests\query25.txt " + $autoTester_relative_path + "tests\out25.xml");
$args_autoTester26 = @($autoTester_relative_path + "tests\simple26.txt " + $autoTester_relative_path + "tests\query26.txt " + $autoTester_relative_path + "tests\out26.xml");
$args_autoTester27 = @($autoTester_relative_path + "tests\simple27.txt " + $autoTester_relative_path + "tests\query27.txt " + $autoTester_relative_path + "tests\out27.xml");
$args_autoTester28 = @($autoTester_relative_path + "tests\affectsBipShortSource.txt " + $autoTester_relative_path + "tests\affectsBipShort.txt " + $autoTester_relative_path + "tests\out28.xml");
$args_autoTester29 = @($autoTester_relative_path + "tests\affectsBipShortExplodeSource.txt " + $autoTester_relative_path + "tests\affectsBipShortExplode.txt " + $autoTester_relative_path + "tests\out29.xml");
$args_autoTester30 = @($autoTester_relative_path + "tests\affectsBipExplodeSource.txt " + $autoTester_relative_path + "tests\affectsBipExplode.txt " + $autoTester_relative_path + "tests\out30.xml");
$args_autoTester31 = @($autoTester_relative_path + "tests\simple31.txt " + $autoTester_relative_path + "tests\query31.txt " + $autoTester_relative_path + "tests\out31.xml");
$args_autoTester32 = @($autoTester_relative_path + "tests\simple32.txt " + $autoTester_relative_path + "tests\query32.txt " + $autoTester_relative_path + "tests\out32.xml");
$args_autoTester33 = @($autoTester_relative_path + "tests\simple33.txt " + $autoTester_relative_path + "tests\query33.txt " + $autoTester_relative_path + "tests\out33.xml");
$args_autoTester34 = @($autoTester_relative_path + "tests\simple34.txt " + $autoTester_relative_path + "tests\query34.txt " + $autoTester_relative_path + "tests\out34.xml");

#change directory to the $solution_path
$current_path = get-location;
chdir($solution_path);

$p = get-location;
write-output $p;

#tests that will be run
#add/remove items if you want to do only unit testing or autoTester testing
#"autoTester11", "autoTester12", "autoTester13" -> contain recursion
#"cppunit", "integration", "autoTester", "autoTester1", "autoTester2", "autoTester3", "autoTester4", "autoTester5", "autoTester6", "autoTester7", "autoTester8", "autoTester9", "autoTester10", "autoTester14", "autoTester15", "autoTester16", "autoTester17", "autoTester18", "autoTester19", "autoTester20", "autoTester21", "autoTester22", "autoTester23", "autoTester24", "autoTester25", "autoTester26", "autoTester27", "autoTester28", "autoTester29", "autoTester30", "autoTester31", "autoTester32", "autoTester33", "autoTester34"
$all_testing = @("autoTester31");
#$all_testing = @("autoTester28", "autoTester29", "autoTester30");

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

