# Mal_Site_Detection

mal_site.txt의 유해사이트 목록을 불러와 http 패킷의 url과 비교하여 사용자가 유해한 사이트에 접속하였는지 탐지.


실행방법
----
    $ sudo python mal_site.py 
    $ sudo python open_url.py


mal_site.py
-----------
mal_site.txt의 유해사이트 목록을 불러와 http 패킷의 url과 비교하여 사용자가 유해한 사이트에 접속하였는지 탐지하고 시간,ip,주소를 txt파일에 저장.

*참고1: 프로그램이 특정 해당사이트를 만나 뻗을때는 56번째 라인의 'Host'부분을 'GET'으로 하드코딩 해주세요.

*참고2
mal_site.py를 실행하고 사이트에 접속한 후의 url과 접속할때의 url이 다른 경우에는 탐지할 수가 없었습니다.(mal_site.txt에 추가하면 가능)

예)	

접속할때 url -> http://SG.yigin.xyz/N  
	
접속후의 url -> http://www.vr832.com


open_url.py
-----------
mal_site.txt의 유해사이트를 불러와 자동으로 브라우저에서 open

*주의:수십개의 유해사이트를 한번에 open하다보니(sleep하였음에도) 브라우저가 뻗거나 컴퓨터가 뻗는 경우가 발생하니 10개씩 끊어서 open하는 것을 권장

