# HTTP Proxy

Данное приложения являтеся http-proxy сервером, используемым для подключения к серверу и скачивания передаваемого видеопотока с одновременным разбиением его на чанки фиксированного размера (60 секунд)

Основными компонентами приложения являются proxy-сервер и клиент. Proxy принимает от клиента запросы вида: 
```
GET "http://<ip-адрес>:<порт>?url=udp://<ip-адрес>:<порт>"
```
часть http://<ip-адрес>:<порт> соответствует серверной части proxy,  
а url=udp://<ip-адрес>:<порт> - сервер, с которого приходит видеопоток

Proxy способен одновременно обрабатывать подключения сразу нескольких клиентов - для каждого клиента в таком случае создаётся отдельная директория, в которую будут сохраняться чанки видео

Ведётся логирование всех основных действий

Соединение между proxy и клиентом устанавливается с помощью протокола TCP, а между proxy и сервером - через UDP

Для разбиения потока на чанки используется FFMPEG API (библиотеки libavformat, avcodec, avutils)

### Зависимости
Для запуска прилоежния необходимо наличие следующих библиотек: [Boost](https://www.boost.org/) и [FFMPEG](https://ffmpeg.org/)

<b>Установка ffmpeg:</b>
```
sudo apt update && sudo apt upgrade
sudo apt install ffmpeg
```
или
```
sudo add-apt-repository ppa:jonathonf/ffmpeg-4
sudo apt update
sudo apt install ffmpeg
```

<b>Установка Boost:</b>
```
sudo apt update
sudo apt install libboost-all-dev
```

### Сборка и Запуск
Для сборки и запуска программы необходимо:
```
git clone git@github.com:SerpentDragon/HTTPProxy.git
cd HTTPProxy
mkdir build
cd build
cmake ..
cmake --build .
```

Запуск proxy осуществляется командой:
```
./proxy <proxy_host> <proxy_port>
```
где <proxy_host> и <proxy_port> - ip-адрес и номер порта, на котором будет запущен proxy

Для запуска клиента требуется выполнить команду:
```
./client <proxy_host> <proxy_port> <server_host> <server_port>
```
где <proxy_host> и <proxy_port> - ip-адрес и порт, на котором запущен proxy,  
а <server_host> и <server_port> - ip-адрес и порт сервера, к которому должен подключиться proxy для чтения видеопотока

### Заметка
Поскольку, отсутствовал полноценный сервер, который мог бы предоставить видеопоток, для его имитации использовался сам ffmpeg, а именно, команда:
```
ffmpeg -re -i <path_to_video> -c copy -f mpegts udp://<host>:<port>
```
