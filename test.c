int main()
{
	
	void *ptr = malloc(1024);
	memset(ptr, 42, 1024);
	write(1, ptr, 1024);

}
