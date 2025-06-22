BEGIN {
    
    m = 2 
    v = 1 
}

{
    x = $1    # x es la primera columna
    
    
    # Con I(x)=(x-m)^2/2*v^2

        I = (x-m)/2*v^2
        print x, I 
    
}
