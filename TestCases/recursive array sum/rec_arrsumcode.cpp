int sum(int x, int y)
{
    if(x>y)
        return 0;
    else
        return (x+sum(x+1, y));
}