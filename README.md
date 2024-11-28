[![License](https://img.shields.io/github/license/Arm-Examples/AWS_MQTT_Demo?label)](https://github.com/Arm-Examples/AWS_MQTT_Demo/blob/main/LICENSE)
[![Build and Execution Test](https://img.shields.io/github/actions/workflow/status/Arm-Examples/AWS_MQTT_Demo/AWS_MQTT-ci.yml?logo=arm&logoColor=0091bd&label=Build%20and%20Execution%20Test)](https://github.com/Arm-Examples/AWS_MQTT_Demo/tree/main/.github/workflows/AWS_MQTT-ci.yml)

# AWS MQTT Demo

This demo application connects to **AWS MQTT broker** using TLS with mutual authentication between the client and the server.
It demonstrates the subscribe-publish workflow of MQTT.

Visit [*coreMQTT mutual authentication demo*](https://docs.aws.amazon.com/freertos/latest/userguide/mqtt-demo-ma.html) for further information. 
Please note, that [*properly configured thing*](https://docs.aws.amazon.com/iot/latest/developerguide/iot-moisture-create-thing.html) is required to
successfully run the demo application.

## Requirements

- [AWS Account](https://aws.amazon.com/free) to connect a [*AWS IoT Thing*](https://docs.aws.amazon.com/iot/latest/developerguide/iot-moisture-create-thing.html).
- A CMSIS-Toolbox enabled toolchain such as [Keil Studio for VS Code](https://www.keil.arm.com/). The [MDK Community Edition](https://www.keil.arm.com/keil-mdk/#mdk-v6-editions) provides all tools required for evaluation.

## Project Structure

This AWS MQTT Mutual Authentication example uses the [CMSIS-Toolbox](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/README.md#cmsis-toolbox)
*csolution project format* with CMSIS software packs and software layers. The default configuration uses a AVH-FVP simulation model.
No physical hardware is required to explore this example. By using different layers it can run on physical evaluation boards,
use different communication stacks, or WiFi modules.

Project File                                                                 | Description
:----------------------------------------------------------------------------|:----------------------------------------------------
[`Demo.csolution.yml`](Demo.csolution.yml)                                   | Specifies the target hardware, build types, and defines the actual software layers used.
[`Demo.cproject.yml`](Demo.cproject.yml)                                     | Contains the source files and components that belong to user application.
[`Socket/.../Socket.clayer.yml`](Socket/VSocket/Socket.clayer.yml)           | Contains the source files and components of the communication interface.
[`Board/.../Board.clayer.yml`](Board/AVH_MPS3_Corstone-300/Board.clayer.yml) | Contains the hardware interfaces to the device and board peripherals.
`Shield/.../Shield.clayer.yml`                                               | Contains the interface source files to an optional Arduino WiFi Shield.

## Configure AWS IoT Thing

To run the demo application [*configure the AWS IoT Thing*](https://docs.aws.amazon.com/iot/latest/developerguide/iot-moisture-create-thing.html) with these steps:

- Modify the following definitions in [config_files/aws_clientcredential.h](amazon-freertos/demos/include/aws_clientcredential.h):
  - `clientcredentialMQTT_BROKER_ENDPOINT`: Remote Host Address (AWS IoT->Settings in AWS IoT console)
  - `clientcredentialIOT_THING_NAME`: Thing Name (AWS IoT->Manage->Things->Name in AWS IoT console)

- Modify the following definitions in [config_files/aws_clientcredential_keys.h](amazon-freertos/demos/include/aws_clientcredential_keys.h):
  - `keyCLIENT_CERTIFICATE_PEM`: Client Certificate
  - `keyCLIENT_PRIVATE_KEY_PEM`: Client Private Key

## Run on AVH FVP Simulation Model

Once the AWS IoT Thing is configured it can be build and run on AVH FVP simulation models.

```bash
cbuild Demo.csolution.yml --context .Debug+AVH --packs

ToDo add FVP command 
```

The execution on AVH FVP simulation models should create this output:

ToDo Show typical output

The MQTT messages can be viewed in the [**AWS IoT console**](https://docs.aws.amazon.com/iot/latest/developerguide/view-mqtt-messages.html).

## [Contiguous Integration (CI)](https://github.com/Arm-Examples/AWS_MQTT_Demo/tree/main/.ci)

The directory [`.ci`](https://github.com/Arm-Examples/AWS_MQTT_Demo/tree/main/.ci) contains [GitHub Actions](https://github.com/Arm-Examples/AWS_MQTT_Demo/tree/main/.github/workflows) for build and execution tests. It executes similar steps on a GitHub Runner.

## Configure for Evaluation Boards

Only when connecting via WiFi Access Point:

- Modify the following definitions in `Socket/WiFi/socket_startup.c`
  - `SSID`:          WiFi Access Point SSID
  - `PASSWORD`:      WiFi Access Point Password
  - `SECURITY_TYPE`: WiFi Access Point Security

## Retarget to Custom Hardware

Application requires setting of the following layers defined by the variables:

- Board-Layer
- Shield-Layer (optional)
- Socket-Layer

To set the board layer (i.e. to specify the "Board-Layer") one shall first install the BSP or DFP for the board in use. To list the name of the available boards use the command:

```bash
csolution list boards
```

This command will also list the respective pack name and version. To continue, follow the instruction on [how to configure the Reference Application](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/ReferenceApplications.md#usage).

>Note: AVH target is already configured with board and variables specified.


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

