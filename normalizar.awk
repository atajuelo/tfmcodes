BEGIN {
    
    total_datos = 1000000 #El total de las frecuencias debe dar 1 si no no es una distribucion
    bin = 0.01 #el ancho de los bines del histograma
}


{
    # Por si dan problemas las columnas 
    if (NF >= 2) {
        x = $1
        y = $2
        # Normalizar la frecuencia 
        y_normalizada = y / (total_datos * bin )
      
        print x, y_normalizada
    }
}
