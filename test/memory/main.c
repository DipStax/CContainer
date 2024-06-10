#include "./Helper.h"
#include "./vector.c"

int main()
{
	fprintf(stderr, "[Memoery Testing] - Start\n");

	fprintf(stderr, "[Memory Testing] Calling test for: . -> main.c\n");
	vector_function();
	printf("[Memory Testing] - Done\n");
	return 0;
}