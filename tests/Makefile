MKDIR_P := mkdir -p
DIR = compiled_test
all: $(DIR)

$(DIR):
	${MKDIR_P} $(DIR)
	cc -arch i386 -c test.c -o $(DIR)/test_i386
	cc -arch i386 -c easy_test.c -o $(DIR)/easy_test_i386
	cc -arch i386 -c easy_test2.c -o $(DIR)/easy_test2_i386

	cc easy_test.c -o $(DIR)/easy_test_obj
	cc easy_test2.c -o $(DIR)/easy_test2_obj
	cc -m32 easy_test2.c -o $(DIR)/easy_test2_32-bit
	cc -arch i386 -arch x86_64 easy_test2.c -o $(DIR)/easy_test2_universal
	cc test.c -o $(DIR)/test_non_external -fvisibility=hidden
	cc test.c -o $(DIR)/test_debug -g

clean:
	rm -rf $(DIR)

re: clean all

