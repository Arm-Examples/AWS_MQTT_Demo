#!/usr/bin/env node

/* -----------------------------------------------------------------------------
 * Copyright (c) 2026 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * -----------------------------------------------------------------------------
 */

// Configure AWS IoT Thing by updating #define placeholders in the config file
// with values from the following environment variables:
//   IOT_THING_NAME           - AWS IoT Thing name
//   MQTT_BROKER_ENDPOINT     - AWS IoT MQTT broker endpoint
//   ROOT_CA_PEM              - Root CA certificate (PEM format)
//   CLIENT_CERTIFICATE_PEM   - Client certificate (PEM format)
//   CLIENT_PRIVATE_KEY_PEM   - Client private key (PEM format)
//
// Usage: node configure-aws-iot-thing.js <input-file> <output-file>

const fs = require("node:fs");
const path = require("node:path");

let inputFilePath;
let outputFilePath;

function getFilePathsFromArgs()
{
    const inputPath = process.argv[2];
    const outputPath = process.argv[3];

    if (!inputPath || !outputPath)
    {
        throw new Error("Missing file path arguments! \nUsage: node configure-aws-iot-thing.js <input-file> <output-file>");
    }

    const resolvedInputPath = path.resolve(process.cwd(), inputPath);

    if (!fs.existsSync(resolvedInputPath))
    {
        throw new Error(`Input file does not exist: ${resolvedInputPath}`);
    }

    if (!fs.statSync(resolvedInputPath).isFile())
    {
        throw new Error(`Input path is not a file: ${resolvedInputPath}`);
    }

    const resolvedOutputPath = path.resolve(process.cwd(), outputPath);

    return [resolvedInputPath, resolvedOutputPath];
}

function getEnvVariable(name)
{
    const value = process.env[name];

    if (value === undefined || value === "")
    {
        throw new Error(`Missing required environment variable: ${name}`);
    }

    return value;
}

function formatStr(value)
{
    return JSON.stringify(value);
}

function formatPEM(value)
{
    const normalizedValue = value.replace(/\\n/g, "\n").replace(/\r\n/g, "\n").replace(/\r/g, "\n");
    const pemValue = normalizedValue.endsWith("\n") ? normalizedValue : `${normalizedValue}\n`;

    return JSON.stringify(pemValue);
}

function setDefine(content, name, value)
{
    const placeholderPattern = new RegExp(
        String.raw`^[ \t]*\* #define ${name}[^\n]*\r?\n[ \t]*\*\/`, "m"
    );

    if (placeholderPattern.test(content))
    {
        return content.replace(placeholderPattern, ` */\n#define ${name} ${value}`);
    }

    throw new Error(`Unable to locate placeholder for ${name} in ${inputFilePath}.`);
}

try
{
    console.log("AWS IoT Thing configuration based on environment variables...");

    [inputFilePath, outputFilePath] = getFilePathsFromArgs();

    const definitions = [
        ["democonfigCLIENT_IDENTIFIER",      formatStr(getEnvVariable("IOT_THING_NAME"))],
        ["democonfigMQTT_BROKER_ENDPOINT",   formatStr(getEnvVariable("MQTT_BROKER_ENDPOINT"))],
        ["democonfigROOT_CA_PEM",            formatPEM(getEnvVariable("ROOT_CA_PEM"))],
        ["democonfigCLIENT_CERTIFICATE_PEM", formatPEM(getEnvVariable("CLIENT_CERTIFICATE_PEM"))],
        ["democonfigCLIENT_PRIVATE_KEY_PEM", formatPEM(getEnvVariable("CLIENT_PRIVATE_KEY_PEM"))],
    ];

    let content = fs.readFileSync(inputFilePath, "utf8");

    for (const [name, value] of definitions)
    {
        content = setDefine(content, name, value);
    }

    fs.writeFileSync(outputFilePath, content, "utf8");

    console.log(`Input:  ${inputFilePath}`);
    console.log(`Output: ${outputFilePath}`);
}
catch (error)
{
    console.error(error.message);
    process.exitCode = 1;
}
