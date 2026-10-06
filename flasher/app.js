javascript
import {
    ESPLoader,
    Transport
} from "https://cdn.jsdelivr.net/npm/esptool-js@0.5.6/+esm";

const connectButton = document.getElementById("connectButton");
const flashButton = document.getElementById("flashButton");

const deviceStatus = document.getElementById("deviceStatus");
const progressBar = document.getElementById("progressBar");
const progressText = document.getElementById("progressText");
const logElement = document.getElementById("log");

let port = null;
let transport = null;
let loader = null;

function log(message) {
    logElement.textContent += `\n${message}`;
    logElement.scrollTop = logElement.scrollHeight;
}

function setStatus(message, connected = false) {
    deviceStatus.innerHTML = `
        <span class="dot"
              style="background:${connected ? "#65d6a6" : "#65716b"}"></span>
        ${message}
    `;
}

connectButton.addEventListener("click", async () => {

    if (!("serial" in navigator)) {
        alert(
            "Web Serial is not supported.\n\n" +
            "Use Google Chrome or Microsoft Edge."
        );
        return;
    }

    try {

        log("Requesting ESP32 serial port...");

        port = await navigator.serial.requestPort();

        transport = new Transport(port);

        loader = new ESPLoader({
            transport,
            baudrate: 115200,
            terminal: {
                clean: () => {},
                writeLine: (data) => log(data),
                write: (data) => log(data)
            }
        });

        setStatus("Connecting to ESP32...", false);

        const chip = await loader.main();

        log(`Chip detected: ${chip}`);

        setStatus(`ESP32 connected · ${chip}`, true);

        flashButton.disabled = false;

        connectButton.textContent = "ESP32 Connected";

    } catch (error) {

        console.error(error);

        log(`[ERROR] ${error.message || error}`);

        setStatus("Connection failed", false);
    }
});

flashButton.addEventListener("click", async () => {

    if (!loader) {
        return;
    }

    try {

        flashButton.disabled = true;

        setStatus("Preparing firmware...", true);

        log("Loading KQ ESP firmware...");

        const response = await fetch(
            "./firmware/kq_esp32.ino.bin"
        );

        if (!response.ok) {
            throw new Error(
                "Firmware file not found."
            );
        }

        const buffer = await response.arrayBuffer();

        const bytes = new Uint8Array(buffer);

        log(
            `Firmware size: ${bytes.length} bytes`
        );

        const fileData = {
            0x10000: bytes
        };

        progressBar.style.width = "0%";
        progressText.textContent = "0%";

        log("Starting flash...");

        await loader.writeFlash({
            fileArray: [
                {
                    data: bytes,
                    address: 0x10000
                }
            ],

            flashSize: "keep",

            reportProgress: (fileIndex, written, total) => {

                const percent =
                    Math.round(
                        (written / total) * 100
                    );

                progressBar.style.width =
                    `${percent}%`;

                progressText.textContent =
                    `${percent}%`;
            }
        });

        log("Flash complete.");

        setStatus(
            "KQ ESP flashed successfully",
            true
        );

        progressBar.style.width = "100%";
        progressText.textContent = "100%";

        log("You can now reset the ESP32.");

    } catch (error) {

        console.error(error);

        log(`[ERROR] ${error.message || error}`);

        setStatus(
            "Flash failed",
            false
        );

    } finally {

        flashButton.disabled = false;
    }
});
