BEGIN {
    
    m = 2 # media 
    v = 1 # varianza 
}

{
    x = $1    # x es la primera columna
    
    
    # Con I(x)=-1+x/m-log(x/m)

        I = -1+x/m-log(x/m)
        print x, I  
    
}
