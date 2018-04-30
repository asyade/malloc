int main()
{
	
	void *ptr = malloc(1024);
	memset(ptr, 42, 16);
	
	write(1, ptr, 1024);

}
