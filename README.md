# WOORDKLOK_WIFI
ESP8266 voor woordklok

Deze library wordt gebruikt in combinatie met de Woordklok
Wel even je eigen WIFI Gegevens invullen (Hardcoded)
Vooralsnog met ESP01 gewerk maar ESP12 kan ook.
TX PIN ESP aansluiten op TX pin board/Blue tooth board (Meest rechtse)

De volgende commando's kunnen gebruikt worden voor het aansturen van de klok:

- http://'IP'/Sound?state=1 -> GELUID AAN
- http://'IP'/Sound?state=0 -> GELUID UIT
- http://'IP'/Notat?state=1 -> TIJDSNOTATIE 1 MIN
- http://'IP'/Notat?state=1 -> TIJDSNOTATIE 5 MIN
- http://'IP'/Time?tijd=12%3A00%3A00 -> Tijd aanpassen (%3A = :)
- http://'IP'/LMax?LMAX=90 -> Max lichtintensiteit 90%
- http://'IP'/LMin?LMIN=5 -> Min lichtintensiteit 5%
- http://'IP'/TComp?TCOMP=-10 -> Tijdscompensatie -10 sec /24 hrs
- http://'IP'/Modes?mode=120 -> Overgangsmodes (110:UIT/120:Waterval/130:R-L/140:R-L/150:FADE OUT-IN/160:FADE IN)

TO DO:

- Fancy webpagina
- Uitlezen gegevens klok en weergeven in webpagina
- AP instellen indien geen wifi bereik
