javascript
import {
    ESPLoader,
    Transport
} from "https://unpkg.com/esptool-js@0.5.7/bundle.js";

const connectButton = document.getElementById("connectButton");
const flashButton = document.getElementById("flashButton");

const deviceStatus = document.getElementById("deviceStatus");
const progressBar = document.getElementById("progressBar");
const progressText = document.getElementById("progressText");
const logElement = document.getElementById("log");

let port = null;
let transport = null;
let loader = null;
let connected = false;
let flashing = false;


// --------------------------------------------------
// LOG
// --------------------------------------------------

function log(message) {
    logElement.textContent += `\n${message}`;
    logElement.scrollTop = logElement.scrollHeight;
}


// --------------------------------------------------
// STATUS
// --------------------------------------------------

function setStatus(message, active = false) {

    const color = active
        ? "#65d6a6"
        : "#65716b";

    deviceStatus.innerHTML = `
        <span
            class="dot"
            style="background:${color}">
        </span>
        ${message}
    `;
}


// --------------------------------------------------
// PROGRESS
// --------------------------------------------------

function setProgress(percent) {

    const value = Math.max(
        0,
        Math.min(100, Math.round(percent))
    );

    progressBar.style.width = `${value}%`;
    progressText.textContent = `${value}%`;
}


// --------------------------------------------------
// ERROR
// --------------------------------------------------

function getErrorMessage(error) {

    if (!error) {
        return "Unknown error";
    }

    if (error.message) {
        return error.message;
    }

    return String(error);
}


// --------------------------------------------------
// CONNECT
// --------------------------------------------------

connectButton.addEventListener("click", async () => {

    if (connected) {
        return;
    }

    if (!("serial" in navigator)) {

        alert(
            "Web Serial is not supported.\n\n" +
            "Use Google Chrome or Microsoft Edge."
        );

        return;
    }

    try {

        connectButton.disabled = true;

        log("Requesting ESP32 serial port...");

        port = await navigator.serial.requestPort();

        log("Serial port selected.");

        transport = new Transport(port, true);

        loader = new ESPLoader({

            transport: transport,

            baudrate: 115200,

            terminal: {

                clean: () => {},

                writeLine: (data) => {
                    log(data);
                },

                write: (data) => {
                    log(data);
                }
            },

            debugLogging: false
        });


        setStatus(
            "Connecting to ESP32...",
            false
        );

        log("Connecting...");

        const chipName = await loader.main();

        log(`Chip detected: ${chipName}`);

        connected = true;

        setStatus(
            `ESP32 connected · ${chipName}`,
            true
        );

        connectButton.textContent =
            "ESP32 Connected";

        flashButton.disabled = false;

        log("Device is ready.");

    } catch (error) {

        console.error(error);

        log(
            `[ERROR] ${getErrorMessage(error)}`
        );

        setStatus(
            "Connection failed",
            false
        );

        connected = false;

        flashButton.disabled = true;

        connectButton.disabled = false;

        if (transport) {

            try {
                await transport.disconnect();
            } catch (_) {}

        }

        port = null;
        transport = null;
        loader = null;
    }
});


// --------------------------------------------------
// FLASH
// --------------------------------------------------

flashButton.addEventListener("click", async () => {

    if (!loader || !connected || flashing) {
        return;
    }

    try {

        flashing = true;

        connectButton.disabled = true;
        flashButton.disabled = true;

        setProgress(0);

        setStatus(
            "Preparing firmware...",
            true
        );

        log("");
        log("================================");
        log("KQ ESP FULL FLASH");
        log("================================");


        // ------------------------------------------
        // LOAD MERGED FIRMWARE
        // ------------------------------------------

        log("Loading KQ ESP merged firmware...");

        const response = await fetch(
            "./firmware/kq_esp32.ino.merged.bin",
            {
                cache: "no-store"
            }
        );

        if (!response.ok) {

            throw new Error(
                `Firmware file not found (${response.status})`
            );
        }

        const buffer =
            await response.arrayBuffer();

        const firmware =
            new Uint8Array(buffer);

        log(
            `Firmware size: ${firmware.length} bytes`
        );

        if (firmware.length === 0) {

            throw new Error(
                "Firmware file is empty."
            );
        }


        // ------------------------------------------
        // ERASE
        // ------------------------------------------

        setStatus(
            "Erasing ESP32 flash...",
            true
        );

        log("Erasing entire flash...");

        await loader.eraseFlash();

        log("Flash erase complete.");


        // ------------------------------------------
        // WRITE
        // ------------------------------------------

        setStatus(
            "Flashing KQ ESP...",
            true
        );

        log(
            "Writing merged firmware at 0x000000..."
        );

        setProgress(0);


        await loader.writeFlash({

            fileArray: [

                {
                    data: firmware,

                    address: 0x000000
                }

            ],

            flashMode: "dio",

            flashFreq: "40m",

            flashSize: "4MB",

            eraseAll: false,

            compress: true,

            reportProgress: (
                fileIndex,
                written,
                total
            ) => {

                const percent =
                    (written / total) * 100;

                setProgress(percent);
            }
        });


        // ------------------------------------------
        // COMPLETE
        // ------------------------------------------

        setProgress(100);

        log("");
        log("Flash complete.");
        log("KQ ESP firmware written successfully.");

        setStatus(
            "KQ ESP flashed successfully",
            true
        );


        // ------------------------------------------
        // RESET
        // ------------------------------------------

        log("Resetting ESP32...");

        try {

            await loader.after("hard_reset");

            log("ESP32 reset complete.");

        } catch (resetError) {

            log(
                `[WARNING] Reset failed: ${
                    getErrorMessage(resetError)
                }`
            );

            log(
                "You can manually press the ESP32 RESET button."
            );
        }


        log("");
        log("================================");
        log("KQ ESP v0.4.2 READY");
        log("================================");


    } catch (error) {

        console.error(error);

        log("");
        log(
            `[ERROR] ${getErrorMessage(error)}`
        );

        setStatus(
            "Flash failed",
            false
        );

        setProgress(0);

    } finally {

        flashing = false;

        connectButton.disabled =
            connected;

        flashButton.disabled =
            !connected;
    }
});


// --------------------------------------------------
// INITIAL STATE
// --------------------------------------------------

setStatus(
    "Waiting for device",
    false
);

setProgress(0);

log(
    "KQ ESP Flasher v0.2"
);

log(
    "Ready."
);

log(
    "Connect your ESP32 via USB."
);
