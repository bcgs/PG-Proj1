# PG-Proj1
## Drawing normal vectors on a Bézier curve

Descrição:<br>O usuário entra via mouse com os pontos de controle de uma curva de Bézier. O número de pontos de controle é arbitrário, sem limite.
O sistema desenha a curva correspondente. O sistema desenha os vetores normais na curva. O vetor normal em questão é uma ortogonalização da segunda derivada em relação à primeira derivada: considere <i>`u`</i> o vetor da primeira derivada, <i>`v`</i> o vetor da segunda derivada para um ponto correspondente a um dado <i>`t`</i>; então o vetor normal para <i>`t`</i> é dado por: <i>`w = v - (<u,v>/<u,u>)u`</i>, onde <i>`<u,v>`</i> é o produto escalar entre <i>`u`</i> e <i>`v`</i>. Se o vetor da primeira derivada for nulo, pode-se fazer <i>`w = 0`</i>. O usuário também poderá determinar o número de avaliações que deverá ser usado para então o sistema calcular os correspondentes pontos da curva e ligá-los por retas. As avaliações tanto da curva como das derivadas deverão ser feitas obrigatoriamente com o Algoritmo de De Casteljau.

## Screenshot
<img src="http://i.imgur.com/eRZxyEm.png" width="450px">
