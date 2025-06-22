BEGIN{

    s_min = -1.0  
    s_max = 1.0   
    s_step = 0.1  # Paso 
    
    n = 200  
    h = 1000000  
 
}

{
    if (NF >= 2) {
        x = $1
        y = $2
        for (s = s_min; s <= s_max; s += s_step) {
            sum_y_exp[s] += y * exp(-s * x * n)
        }
        total_weight += y
    }
}

END {
    for (s = s_min; s <= s_max; s += s_step) {
        MGF = sum_y_exp[s] / total_weight
        theta = log(MGF) / n
        print s, theta
    }
}


