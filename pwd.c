#include <stdio.h>
#include <unistd.h>
int main(void)
{
	char buf[BUFSIZ];
	
	getcwd(buf, BUFSIZ);
	printf("%s\n", buf);
}
