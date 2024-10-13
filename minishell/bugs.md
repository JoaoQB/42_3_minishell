//

Test 146: ❌ README.md 
mini exit code = 126
bash exit code = 127
mini error = ( Permission denied)
bash error = ( command not found)

//this file exists but is not an executable

Test 140: ❌ test_files 
mini exit code = 126
bash exit code = 127
mini error = ( Is a directory)
bash error = ( command not found)

//its an directory, but aint detecting as one (cuz of no /)

Test 104: ❌ cat <missing >./outfiles/outfile01 
mini exit code = 0
bash exit code = 1

//i do get 1 with no file or directory


Test  79: ❌ cat <missing | cat <"./test_files/infile" 
mini output = ()
bash output = (hi hello world 42)

//i get no sutch file or dir but return as 0...
//also 1 error is suposed to not afect next