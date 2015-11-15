int  fun(int n)
{
	if(n>1)
	return fun(n-1)+fun(n-2);
	else if(n==1)
	return 1;
	else 
	return 0;
}
int main()
{
	fun(5);
}