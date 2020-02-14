import configparser

config = configparser.ConfigParser()
config.read('config.conf')

username = config.get('Global', 'username')
print(username)