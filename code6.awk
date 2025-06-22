BEGIN {

    n = 200 
}

{
    x = $1    # x es la primera columna
    p = $2    # p(x) es la segunda
    
    # Calculamos I(x) usando la relación I(x) = - (1/n) * ln(p(x))
    if (p > 0) {
        I = - (1 / n) * log(p)
        print x, I  
    } else {
        print x, "undefined"  # Si p(x) es 0 o negativo, I(x) no está definida
    }
}
