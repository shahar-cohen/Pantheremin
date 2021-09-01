from pythonosc.osc_server import AsyncIOOSCUDPServer
from pythonosc.udp_client import SimpleUDPClient
from pythonosc.dispatcher import Dispatcher
import asyncio

note_map = {46:1,48:2,51:3,53:4,55:5,58:6,60:7,63:8}

client = SimpleUDPClient("10.0.0.10", 57120)

ip = "10.0.0.13"
port = 8338


def filter_handler(address, *args):
    if args[0] > 0:
        payload = args[0] if address[1] == "4" else note_map[args[0]]
        client.send_message(address, payload)


dispatcher = Dispatcher()
dispatcher.map("/*", filter_handler)



async def loop():
    while(True):
        await asyncio.sleep(1)


async def init_main():
    server = AsyncIOOSCUDPServer((ip, port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint()

    await loop()

    transport.close() 


asyncio.run(init_main())
