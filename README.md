Task4 and Task5 need root permission!

The sort function has special feature, please check Task1 part and Task5 part in reprot.

If Makefile doesn't work, use command below to compile and run code:
Task1:
	g++ Main.cpp -o Task1filter.exe -Wall -g
	./Task1filter.exe DirtyFile Cleanfile

Task2: 
	g++ Main2.cpp -o Task2.exe -Wall -g
	./Task2.exe InputFile OutputFile

Task3: 
	g++ Main3.cpp -o Task3.exe -Wall -g
	./Task3.exe InputFile OutputFile

Task4: 
	g++ Main4.cpp -o Task4.exe -Wall -g
	./Task4.exe InputFile OutputFile

Task5: 
	g++ Main5.cpp -o Task5.exe -Wall -g
	cat InputFile | ./Task5.exe OutputFile