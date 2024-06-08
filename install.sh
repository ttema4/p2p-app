#!/bin/bash

brew update

brew install qt6

brew install pyenv

brew install pyenv-virtualenv

echo 'export PYENV_ROOT="$HOME/.pyenv"' >> ~/.zshrc
echo 'export PATH="$PYENV_ROOT/bin:$PATH"' >> ~/.zshrc
echo 'eval "$(pyenv init --path)"' >> ~/.zshrc
echo -e 'if command -v pyenv 1>/dev/null 2>&1; then\n  eval "$(pyenv init -)"\nfi' >> ~/.zshrc

pyenv install 3.8
pyenv install 3.12

pyenv virtualenv 3.12 bybit_scraper
pyenv virtualenv 3.8 htx_scraper

cd '/Users/exchange scraper_/bybit scraper'
pyenv local bybit_scraper
/Applications/Python\ 3.12.3/Install\ Certificates.command
pip install pybit pycryptodome requests aiohttp

cd '/Users/exchange scraper_/htx scraper'
pyenv local htx_scraper
/Applications/Python\ 3.8.19/Install\ Certificates.command
pip install aiohttp