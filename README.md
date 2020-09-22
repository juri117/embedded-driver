# embedded-driver

haredware driver for esp32 and raspberry pi to enable portable code

## dependencies

### esp32

* the only dependency is the [esp-idf](https://github.com/espressif/esp-idf)

### pi

* to store settings [rapidjson](https://github.com/Tencent/rapidjson) is used

## known issues

* lack of documentation
* pi: TaskEventVar::wait_for_one does not wait for any of the given bits but just for the first one
* pi: DelayUntil::wait_for just does a simple rel. wait