N = 1e6;          % número de muestras
n = 200;          % variables por muestra
s = -0.1;         % parámetro tilting

mu_p = 2.0;       % media p(x)
mu_ref = 1.5;     % media p_ref(x)
sigma_p = 1.0;
sigma_pref = 1.0;

% Generar muestras desde p_ref
X = randn(N, n) * sigma_pref + mu_ref;

% Calcular S_n
Sn = mean(X, 2);

% h(x)
h = exp(-s * n * Sn);

% log de densidades
log_p = -0.5 * ((X - mu_p).^2) / sigma_p^2;
log_pref = -0.5 * ((X - mu_ref).^2) / sigma_pref^2;

% log cociente
log_ratio = sum(log_p - log_pref, 2);

% pesos importancia
w = exp(log_ratio);

% función f = w * h
f = w .* h;

% Estimador de Z_n(s)
Z_est = mean(f);

% Varianza absoluta
var_abs = mean(f.^2) - Z_est^2;

% Desviación estándar
std_abs = sqrt(var_abs);

% Varianza relativa y error relativo
var_rel = var_abs / (Z_est^2);
std_rel = std_abs / Z_est;

fprintf('Estimador Z_n(s) = %.6e\n', Z_est);
fprintf('Varianza absoluta = %.6e\n', var_abs);
fprintf('Desviación estándar absoluta = %.6e\n', std_abs);
fprintf('Varianza relativa = %.6e\n', var_rel);
fprintf('Error relativo (desviación/std) = %.6e\n', std_rel);
