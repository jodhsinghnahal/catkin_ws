import asyncio
from gmqtt import Client as MQTTClient

# Constants
BROKER_ADDRESS = 'localhost'
BROKER_PORT = 1883
TOPIC = 'xnet/#'

# Callback when the client connects to the broker
async def on_connect(client, flags, rc, properties):
    print('Connected to the broker')
    client.subscribe(TOPIC, qos=1)

# Callback when a message is received
async def on_message(client, topic, payload, qos, properties):
    if 'invchg1/' in topic: 
        print(f'Received message: {payload.decode()} on topic: {topic}')

# Callback when the client disconnects from the broker
async def on_disconnect(client, packet, exc=None):
    print('Disconnected from the broker')

def on_connect_sync(client, flags, rc, properties):
    asyncio.ensure_future(on_connect(client, flags, rc, properties))

def on_message_sync(client, topic, payload, qos, properties):
    asyncio.ensure_future(on_message(client, topic, payload, qos, properties))

def on_disconnect_sync(client, packet, exc=None):
    asyncio.ensure_future(on_disconnect(client, packet, exc))

async def main():
    client = MQTTClient("gmqtt-subscriber")

    client.on_connect = on_connect_sync
    client.on_message = on_message_sync
    client.on_disconnect = on_disconnect_sync

    await client.connect(BROKER_ADDRESS, BROKER_PORT)

    try:
        await asyncio.Event().wait()
    finally:
        await client.disconnect()

if __name__ == '__main__':
    asyncio.run(main())


