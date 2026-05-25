#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t char_in_set(const char c, const char* char_set) {
	while (*char_set) {
		if (*char_set == c)
			return 1;
		char_set++;
	}
	return 0;
}

uint32_t char_count_in(const char c, const char* src) {
	int counter=0;
	while (*src) {
		if (*src == c) counter++;
		src++;
	}
	return counter;
}

char *multiply_character_in_string(uint16_t count, const char *char_list, const char *str) {
	// contor
	uint32_t occurances=0;
	const char* char_enum=char_list;
	while (*char_enum) {
		occurances += char_count_in(*char_enum, str);
		char_enum++;
	}

	// alloc
	char *buff = malloc(
		(strlen(str) + occurances*count + 1)  // +1 = trailing \0'
		* sizeof(char)
	);
	
	char *enumerator = buff;
	*enumerator = 0;
	
	// build buffer
	while (*str) {
		*enumerator = *str;
		enumerator++;

		// multiply
		if (char_in_set(*str, char_list) && (count > 0)) {
			memset(enumerator, *str, count);
			enumerator += count;
		}

		//
		str++;
	}
	*enumerator = '\0';

	//
	return buff;
}

#define BUF_DEFAULT 256

char* read_string_from_io(char* prompt) {
	char* res=malloc(BUF_DEFAULT * sizeof(char));
	size_t ressize=BUF_DEFAULT, pos = 0;
	char buff[256];
	size_t cnt, copyto;
	printf("start\n");

	// Prompt & read
	printf("%s=", prompt);
	while (fgets(&buff[0], 256, stdin) != NULL) {
		cnt = strlen(buff);
		if (cnt == 1) // just a newline
			break;
		copyto = cnt-1;
		if (buff[copyto] != '\n')
			copyto++;

		// Re-alloc
		if (pos+copyto+1 > ressize) {
			ressize *= 2;
			res = realloc(res, ressize);
		}

		// Copy
		//printf("\n\nsa citit=%s\nsa copiat=%lu", &buff[0], copyto);
		strncpy(&res[pos], &buff[0], copyto);
		pos += copyto+1;
		
		// Check is newline
		if (buff[cnt-1] == '\n')
			break;
	}
	res[pos-1] = '\0';
	
	return res;
}

int main() {
	char *char_list;
	uint16_t count;
	char *str;
	char *res = NULL;

	// Read from IO
	char_list = read_string_from_io("char_list");	
	printf("count="); scanf("%hu", &count);
	fgetc(stdin);
	str = read_string_from_io("str");

	// Exec
	res = multiply_character_in_string(count, char_list, str);
	
	printf("\nRezultat:\n");
	if (res != NULL)
		printf("%s\n", res);

	// free
	if (char_list != NULL)
		free(char_list);
	if (str != NULL)
		free(str);
	if (res != NULL)
		free(res);

	// Done
	printf("\nDone!\n");
}
