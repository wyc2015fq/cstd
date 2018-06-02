
int i, j;
int rows = asz.len[0];
int cols = asz.len[1];

if (dimension == 0)
{
    for (j = 0; j < cols; j++)
    {
        s = 0;
        for (i = 0; i < rows; i++)
            s += a[i*al+j];
        sum[j] = s;
    }
}
else if (dimension == 1)
{
    for (j = 0; j < rows; j++)
    {
        s = 0;
        for (i = 0; i < cols; i++)
            s += a[j*al+i];
        sum[j] = s;
    }
}
else
{
    ASSERT(0 && "Invalid dimension");
}
