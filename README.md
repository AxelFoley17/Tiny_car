# <p align="center"> Önvezető kiskocsi </p>

## Főbb alkotórészek

* lpc 845 brk mikrokontroller és board
* ultrahangos távolságszenzor https://www.hestore.hu/prod_10035534.html
* kettős h híd https://www.hestore.hu/prod_10037969.html
* csomagban vehető váz, kerekekkel, dc motorokkal
* 2 db optikai szenzor https://www.hestore.hu/prod_10041643.html
<br><br>

## Működő funkciók
* távolságszenzorral való távolság mérés
* h híddal való motorvezérlés PWM-mel
* előre haladás
* ha 30 cm-nél közelebb van valami, akkor megáll
<br><br>

## Tervezett funkciók
* önvezetés
	1. előre haladás
	2. bizonyos távolságonként egy α szögben végigpásztázás
	3. tovább haladás úgy, hogy a legnagyobb távolságú α szögben lévő pont felé menjen
	4. vissza az 1. pontra
<br><br>
* önszabályozás
	* nézi a kerekeinek a tényleges sebességét
	* növeli / csökkenti a pwm-et, hogy a kerekek sebessége a kívánt legyen
<br><br>
* vezeték nélküli modullal telenfonról irányítás
	* https://www.hestore.hu/prod_10035525.html
<br><br>

## A repo tartalma
* ez a repo egy MCUXpresso IDE workspace-t tartalmaz aminek az egyetlen projektje a kiskocsi kódja