# Mal_Site_Detection

mal_site.txt의 유해사이트 목록을 불러와 http 패킷의 url과 비교하여 사용자가 유해한 사이트에 접속하였는지 탐지.

mal_site.py와 arp_attack.py를 같이 실행시킨다.

실행방법
----
    $ sudo python mal_site.py 
    $ sudo python arp_attack.py


mal_site.py
-----------
mal_site.txt의 유해사이트 목록을 불러와 http 패킷의 url과 비교하여 사용자가 유해한 사이트에 접속하였는지 탐지하고 시간,ip,주소를 txt파일에 저장.


*참고1
mal_site.py를 실행하고 사이트에 접속한 후의 url과 접속할때의 url이 다른 경우에는 탐지할 수가 없었습니다.(mal_site.txt에 추가하면 가능)

예)	

접속할때 url -> http://SG.yigin.xyz/N  
	
접속후의 url -> http://www.vr832.com


arp_attack.py
-----------
arp 패킷을 주기적으로 보내서 감염시키는 스크립트 => mal_site.py에 있는 감염 기능을 빼고 새로 만들었다 이유는 mal_site.py가 너무 느려서 따로 구현하였다.