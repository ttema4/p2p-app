# Описание структуры данных для храннеия ордеров

Зайдя на bybit, в раздел p2p, понимаем, что можем вывести ордера для фильтра по нескольким банкам и одной монеты(которую покупаем), но не можем наоборот - ордера для покупки любой из нескольких монет с оплатой через выбранный банки/банки.

Следовательно, после парсинга имеем что-то вроде:

BUY - ID - РЕЙТИНГ ПРОДАВЦА - RUB(DEFAULT) - USDT - ОДИН ИЗ 6(выбранных нами) БАНКОВ - ОБЪЕМ(РУБ) - КУРС
