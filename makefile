
# Group 19 Compiler Construction Submission
# 	Rajat Payghan (2021A7PS2218P)
# 	Rudra Goyal (2021A7PS0708P)
# 	Samraddh Saxena (2021A7PS1455P)
# 	Aditya Patel (2021A7PS2433P)
# 	Bhagwati Iyer (2021A7PS0685P)
# 	Jai Bothra (2021A7PS0015P)

parser:
	# rm parser.exe
	gcc -o stage1exe driver.c parser.c lexer.c
	./stage1exe text.txt parserOutput.txt