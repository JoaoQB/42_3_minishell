Test  75: ❌ # cat <"./test_files/infile_big" | echo hi
mini output = (hi)
bash output = ()
mini error = ( command not found)
bash error = ()


Test 133: ✅⚠️  $PWD
mini error = ( Is a directory Is a directory)
bash error = ( Is a directory)


Test 137: ❌ ./missing.out
mini exit code = 126
bash exit code = 127
mini error = ( No such file or directory Is a directory)
bash error = ( No such file or directory)


Test 140: ❌ test_files
mini exit code = 13
bash exit code = 127
mini error = ( Permission denied)
bash error = ( command not found)


Test 141: ✅⚠️  ./test_files
mini error = ( Is a directory Is a directory)
bash error = ( Is a directory)


Test 142: ❌ /test_files
mini exit code = 126
bash exit code = 127
mini error = ( No such file or directory Is a directory)
bash error = ( No such file or directory)


Sometimes random errors like:

Test 115: ❌ ls >>./test_files/invalid_permission
mini exit code = 0
bash exit code = 1
