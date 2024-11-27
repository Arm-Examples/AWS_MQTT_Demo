# AWS MQTT Demo

AWS MQTT Mutual Authentication example using CMSIS solution layers.

This demo application connects to **AWS MQTT broker** using TLS with mutual authentication between the client and the server.
It demonstrates the subscribe-publish workflow of MQTT.

Visit [*coreMQTT mutual authentication demo*](https://docs.aws.amazon.com/freertos/latest/userguide/mqtt-demo-ma.html) for further information.

Please note, that [*properly configured thing*](https://docs.aws.amazon.com/iot/latest/developerguide/iot-moisture-create-thing.html) is required to
successfully run the demo application.

## Target

 - AVH: [Arm Virtual Hardware for Corstone-300](./Board/AVH_MPS3_Corstone-300/README.md)
 - MyBoard: Board with compatible board layer

Application requires setting of the following layers defined by the variables:
  - Board-Layer
  - Shield-Layer (optional)
  - Socket-Layer

To set the board layer (i.e. to specify the "Board-Layer") one shall first install the BSP or DFP for the board in use. To list the name of the available boards use the command:
```
csolution list boards
```
This command will also list the respective pack name and version. To continue, follow the instruction on [how to configure the Reference Application](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/ReferenceApplications.md#usage).

>Note: "board" and "Board-Layer" are already specified for the AVH target.

## Configure

Configure AWS IoT Thing:
  - Modify the following definitions in [aws_clientcredential.h](amazon-freertos/demos/include/aws_clientcredential.h):
    - `clientcredentialMQTT_BROKER_ENDPOINT`: Remote Host Address (AWS IoT->Settings in AWS IoT console)
    - `clientcredentialIOT_THING_NAME`: Thing Name (AWS IoT->Manage->Things->Name in AWS IoT console)
  - Modify the following definitions in [aws_clientcredential_keys.h](amazon-freertos/demos/include/aws_clientcredential_keys.h):
    - `keyCLIENT_CERTIFICATE_PEM`: Client Certificate
    - `keyCLIENT_PRIVATE_KEY_PEM`: Client Private Key

Configure WiFi Access Point (when connecting via WiFi):
  - Modify the following definitions in `socket_startup.c`
    - `SSID`:          WiFi Access Point SSID
    - `PASSWORD`:      WiFi Access Point Password
    - `SECURITY_TYPE`: WiFi Access Point Security

## Build

```
cbuild Demo.csolution --context Demo.Debug+<target-type>
```

## Program

- Download the executable file (.axf) to the microcontroller using a programmer or Drag-and-drop programming if available.
>Note: not required for Virtual Hardware.

## Run

- Connect and configure the debugger.
- Run the application and view messages in a debug printf or terminal window.

MQTT messages can be viewed in the [**AWS IoT console**](https://docs.aws.amazon.com/iot/latest/developerguide/view-mqtt-messages.html).
