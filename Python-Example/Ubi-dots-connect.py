from ubidots import ApiClient

# API KEY QUE É RETIRADA DO SITE
api = ApiClient("BBFF-4b4301b0ba2475c7225443fd2a004ce3bd4")

# PEGA O ID DO DEVICE QUE VAI SER UTILIZADO
my_device = api.get_variable('61f95bfc1d84725255c88fa8')

last_value = my_device.get_values()
print(last_value)

