import json
from huobi.client.market import MarketClient
from huobi.utils import *
import asyncio
import aiohttp
import socket

coins = {"USDT": "2", "BTC": "1", "ETH": "3"}
orders = list()
spot_rates = dict()


async def scrap_p2p(tradeType, coin):
    async with aiohttp.ClientSession() as session:
        for currPage in range(1, 4):
            async with session.get(
                f"https://www.htx.com/-/x/otc/v1/data/trade-market?coinId={coins[coin]}&currency=11&tradeType={tradeType}&currPage={str(currPage)}&payMethod=0&acceptOrder=0&country=&blockType=general&online=1&range=0&amount=&onlyTradable=false&isFollowed=false"
            ) as response:
                result = await response.json()
                for order_ in result["data"]:
                    order = dict()
                    order["market"] = "htx"
                    order["currency"] = "RUB"
                    order["coin"] = coin
                    if tradeType == "sell":
                        order["direction"] = "buy"
                    else:
                        order["direction"] = "sell"
                    order["price"] = order_["price"]
                    order["payment_methods"] = list()
                    for payMethod in order_["payMethods"]:
                        order["payment_methods"].append(payMethod["name"])
                    order["lower_limit"] = order_["minTradeLimit"]
                    order["upper_limit"] = order_["maxTradeLimit"]
                    order["available"] = order_["tradeCount"]
                    order[
                        "link"
                    ] = "https://www.htx.com/en-us/fiat-crypto/trader/" + str(
                        order_["uid"]
                    )
                    orders.append(order)


def scrap_spot():
    market_client = MarketClient()
    coins_ = ["usdt", "btc", "eth"]
    for i in range(3):
        for j in range(i + 1, 3):
            list_obj = market_client.get_history_trade(coins_[j] + coins_[i], 1)
            spot_rates[coins_[j].upper() + "/" + coins_[i].upper()] = str(list_obj[0].price)
    spot_rates["market"] = "htx"


async def main_():
    scraping_tasks = []
    for tradeType in ["sell", "buy"]:
        for coin in ["BTC", "ETH", "USDT"]:
            scraping_tasks.append(asyncio.create_task(scrap_p2p(tradeType, coin)))
    await asyncio.gather(*scraping_tasks)


def main():
    sock = socket.socket()
    sock.bind(("127.0.0.1", 12444))
    sock.listen(1)
    conn, addr = sock.accept()
    asyncio.run(main_())
    scrap_spot()
    data_market = dict()
    data_market["orders"] = orders
    data_market["spot_rates"] = spot_rates
    conn.send(json.dumps(data_market).encode())
    print(data_market)
    conn.close()
    orders.clear()


main()