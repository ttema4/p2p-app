import asyncio
import json
from pybit.unified_trading import HTTP
import aiohttp
import socket

url = "https://api2.bybit.com/fiat/otc/item/online"
directions = {"0": "sell", "1": "buy"}
coins = ["USDT", "USDC", "BTC", "ETH"]
payments = {
    "51": "Payeer",
    "582": "Sberbank",
    "581": "Tinkoff",
    "1": "A-Bank",
    "595": "OZON Bank",
    "382": "VTB Bank",
    "64": "Raiffeisenbank",
    "49": "OTP Bank",
}
orders = list()
spot_rates = dict()


async def scrap_p2p(side, coin):
    part_of_orders = list()
    data = {
        "userId": "",
        "tokenId": coin,
        "currencyId": "RUB",
        "payment": [],
        "side": str(side),
        "size": "30",
        "page": "1",
        "amount": "",
        "authMaker": False,
        "canTrade": False,
    }
    async with aiohttp.ClientSession() as session:
        async with session.post(url, json=data) as response:
            result = await response.json()
            for order_ in result["result"]["items"]:
                order = dict()
                order["market"] = "bybit"
                order["currency"] = order_["currencyId"]
                order["coin"] = coin
                order["direction"] = directions[str(side)]
                order["price"] = order_["price"]
                order["payment_methods"] = list()
                for payment in order_["payments"]:
                    try:
                        order["payment_methods"].append(payments[payment])
                    except KeyError:
                        continue
                if len(order["payment_methods"]) == 0:
                    continue
                order["lower_limit"] = order_["minAmount"]
                order["upper_limit"] = order_["maxAmount"]
                order["available"] = order_["quantity"]
                order["link"] = (
                    "https://www.bybit.com/fiat/trade/otc/profile/"
                    + order_["userId"]
                    + "/"
                    + coin
                    + "/"
                    + order_["currencyId"]
                    + "/item"
                )
                part_of_orders.append(order)
    orders.extend(part_of_orders)


def scrap_spot():
    session = HTTP()
    for i in range(0, 4):
        for j in range(i + 1, 4):
            response = session.get_orderbook(category="spot", symbol=(coins[j] + coins[i]))
            spot_rates[coins[j] + "/" + coins[i]] = response["result"]["b"][0][0]
    spot_rates["market"] = "bybit"


async def main():
    scraping_tasks = []
    for i in range(0, 2):
        for coin in coins:
            scraping_tasks.append(asyncio.create_task(scrap_p2p(i, coin)))
    await asyncio.gather(*scraping_tasks)


while True:
    sock = socket.socket()
    sock.bind(("127.0.0.1", 12344))
    sock.listen(1)
    conn, addr = sock.accept()
    asyncio.run(main())
    scrap_spot()
    data_market = dict()
    data_market["orders"] = orders
    data_market["spot_rates"] = spot_rates
    conn.send(json.dumps(data_market).encode())
    conn.close()
    orders.clear()