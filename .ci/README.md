# Continuous Integration (CI)

Content of `.ci` Directory   | Description
:----------------------------|:-----------------
`vcpkg-configuration.json`   | Tool setup for the CI test

The GitHub Actions in the directory [`.github/workflows`](https://github.com/Arm-Examples/AWS_MQTT_Demo/tree/main/.github/workflows) are the scripts for the CI tests. These scripts contain detailed comments about each step that is executed.

## GitHub Secrets - Values

The following GitHub Secrets need to be added to a repository [Secret store](../../settings/secrets/actions) to connect a [*AWS IoT Thing*](https://docs.aws.amazon.com/iot/latest/developerguide/iot-moisture-create-thing.html).

GitHub Secret                  | Enables *AWS IoT Thing* Connection
:------------------------------|:---------------------------------------
`CLIENT_CERTIFICATE_PEM`       | Client (device) certificate
`CLIENT_PRIVATE_KEY_PEM`       | Client (device) private key
`IOT_THING_NAME`               | Client  (device) name
`MQTT_BROKER_ENDPOINT`         | MQTT broker host name
