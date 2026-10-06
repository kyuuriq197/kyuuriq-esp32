javascript
import {
    ESPLoader,
    Transport
} from "https://unpkg.com/esptool-js@0.5.7/bundle.js";


// --------------------------------------------------
// ELEMENTS
// --------------------------------------------------

const connectButton =
    document.getElementById("connectButton");

const reconnectButton =
    document.getElementById("reconnectButton");

const flashButton =
    document.getElementById("flashButton");

const clearLogButton =
    document.getElementById("clearLogButton");

const deviceStatus =
    document.getElementById("deviceStatus");

const portInfo =
    document.getElementById("portInfo");

const statusDot =
    document.getElementById("statusDot");

const progressBar =
    document.getElementById("progressBar");

const progressText =
    document.getElementById("progressText");

const progressStage =
    document.getElementById("progressStage");

const flashStage =
    document.getElementById("flashStage");

const logElement =
    document.getElementById("log");


// --------------------------------------------------
// STATE
// --------------------------------------------------

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

    logElement.scrollTop =
        logElement.scrollHeight;
}


// --------------------------------------------------
// STATUS
// --------------------------------------------------

function setStatus(
    message,
    active = false
) {

    deviceStatus.textContent =
        message;

    statusDot.classList.toggle(
        "active",
        active
    );
}


// --------------------------------------------------
// PORT INFO
// --------------------------------------------------

function setPortInfo(message) {

    portInfo.textContent =
        message;
}


// --------------------------------------------------
// PROGRESS
// --------------------------------------------------

function setProgress(percent) {

    const value = Math.max(
        0,
        Math.min(
            100,
            Math.round(percent)
        )
    );

    progressBar.style.width =
        `${value}%`;

    progressText.textContent =
        `${value}%`;
}


// --------------------------------------------------
// STAGE
// --------------------------------------------------

function setStage(message) {

    flashStage.textContent =
        message;

    progressStage.textContent =
        message;
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
// BROWSER CHECK
// --------------------------------------------------

function checkWebSerial() {

    if (!("serial" in navigator)) {

        setStatus(
            "Web Serial unavailable",
            false
        );

        setPortInfo(
            "Use Google Chrome or Microsoft Edge"
        );

        connectButton.disabled = true;
        reconnectButton.disabled = true;

        log(
            "[ERROR] Web Serial is not supported."
        );

        log(
            "Use Chrome or Microsoft Edge over HTTPS."
        );

        return false;
    }

    return true;
}


// --------------------------------------------------
// CONNECT TO PORT
// --------------------------------------------------

async function connectToPort(
    selectedPort
) {

    try {

        port = selectedPort;

        connectButton.disabled = true;
        reconnectButton.disabled = true;

        setStatus(
            "Connecting to ESP32...",
            false
        );

        setPortInfo(
            "Opening serial connection..."
        );

        log(
            "Opening serial connection..."
        );


        // ------------------------------------------
        // TRANSPORT
        // ------------------------------------------

        transport =
            new Transport(
                port,
                true
            );


        // ------------------------------------------
        // ESPTOOL
        // ------------------------------------------

        loader =
            new ESPLoader({

                transport,

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


        log(
            "Connecting to ESP32 bootloader..."
        );


        // ------------------------------------------
        // DETECT CHIP
        // ------------------------------------------

        const chipName =
            await loader.main();


        log(
            `Chip detected: ${chipName}`
        );


        connected = true;


        setStatus(
            `ESP32 connected · ${chipName}`,
            true
        );


        setPortInfo(
            getPortDescription(port)
        );


        connectButton.textContent =
            "ESP32 Connected";

        flashButton.disabled = false;

        reconnectButton.disabled = false;

        setStage(
            "ESP32 ready for flashing"
        );


        log(
            "Device is ready."
        );

    } catch (error) {

        console.error(error);

        log(
            `[ERROR] ${getErrorMessage(error)}`
        );

        setStatus(
            "Connection failed",
            false
        );

        setPortInfo(
            "Check USB cable and try again"
        );

        connected = false;

        flashButton.disabled = true;

        connectButton.disabled = false;
        reconnectButton.disabled = false;

        await cleanupConnection();
    }
}


// --------------------------------------------------
// PORT DESCRIPTION
// --------------------------------------------------

function getPortDescription(
    serialPort
) {

    if (!serialPort) {
        return "USB serial device";
    }

    const info =
        serialPort.getInfo();

    const parts = [];

    if (info.usbVendorId) {

        parts.push(
            `VID 0x${info.usbVendorId
                .toString(16)
                .toUpperCase()
                .padStart(4, "0")}`
        );
    }

    if (info.usbProductId) {

        parts.push(
            `PID 0x${info.usbProductId
                .toString(16)
                .toUpperCase()
                .padStart(4, "0")}`
        );
    }

    if (parts.length === 0) {
        return "USB serial device";
    }

    return parts.join(" · ");
}


// --------------------------------------------------
// SELECT USB DEVICE
// --------------------------------------------------

connectButton.addEventListener(
    "click",
    async () => {

        if (connected || flashing) {
            return;
        }

        if (!checkWebSerial()) {
            return;
        }

        try {

            log(
                "Opening USB device selector..."
            );

            /*
             * IMPORTANT:
             *
             * requestPort() is called directly
             * from the button click.
             *
             * This allows the browser to open
             * its native serial-device chooser.
             */

            const selectedPort =
                await navigator.serial.requestPort();

            log(
                "USB serial device selected."
            );

            await connectToPort(
                selectedPort
            );

        } catch (error) {

            /*
             * User cancelled the browser dialog.
             */

            if (
                error &&
                error.name ===
                "NotFoundError"
            ) {

                log(
                    "Port selection cancelled."
                );

                return;
            }

            console.error(error);

            log(
                `[ERROR] ${getErrorMessage(error)}`
            );
        }
    }
);


// --------------------------------------------------
// RECONNECT
// --------------------------------------------------

reconnectButton.addEventListener(
    "click",
    async () => {

        if (connected || flashing) {
            return;
        }

        if (!checkWebSerial()) {
            return;
        }

        try {

            log(
                "Searching for previously permitted devices..."
            );

            const ports =
                await navigator.serial.getPorts();


            if (ports.length === 0) {

                log(
                    "No previously permitted device found."
                );

                log(
                    "Opening USB device selector..."
                );

                const selectedPort =
                    await navigator.serial.requestPort();

                await connectToPort(
                    selectedPort
                );

                return;
            }


            /*
             * If the browser remembers a device,
             * reconnect to the first permitted port.
             */

            log(
                `Found ${ports.length} permitted device(s).`
            );

            await connectToPort(
                ports[0]
            );

        } catch (error) {

            console.error(error);

            log(
                `[ERROR] ${getErrorMessage(error)}`
            );

            setStatus(
                "Reconnect failed",
                false
            );
        }
    }
);


// --------------------------------------------------
// FLASH
// --------------------------------------------------

flashButton.addEventListener(
    "click",
    async () => {

        if (
            !loader ||
            !connected ||
            flashing
        ) {
            return;
        }

        try {

            flashing = true;

            connectButton.disabled = true;
            reconnectButton.disabled = true;
            flashButton.disabled = true;


            // --------------------------------------
            // START
            // --------------------------------------

            setProgress(0);

            setStage(
                "Preparing firmware..."
            );

            setStatus(
                "Preparing firmware...",
                true
            );

            log("");
            log("================================");
            log("KQ ESP FULL FLASH");
            log("================================");


            // --------------------------------------
            // LOAD FIRMWARE
            // --------------------------------------

            log(
                "Loading KQ ESP merged firmware..."
            );

            const response =
                await fetch(
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


            // --------------------------------------
            // ERASE
            // --------------------------------------

            setStage(
                "Erasing flash..."
            );

            setStatus(
                "Erasing ESP32 flash...",
                true
            );

            log(
                "Erasing entire flash..."
            );

            setProgress(0);


            await loader.eraseFlash();


            log(
                "Flash erase complete."
            );


            // --------------------------------------
            // WRITE
            // --------------------------------------

            setStage(
                "Writing firmware..."
            );

            setStatus(
                "Flashing KQ ESP...",
                true
            );

            log(
                "Writing merged firmware at 0x000000..."
            );


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

                    if (total <= 0) {
                        return;
                    }

                    const percent =
                        (written / total) * 100;

                    setProgress(
                        percent
                    );
                }
            });


            // --------------------------------------
            // COMPLETE
            // --------------------------------------

            setProgress(100);

            setStage(
                "Flash complete"
            );

            log("");
            log(
                "Flash complete."
            );

            log(
                "KQ ESP firmware written successfully."
            );

            setStatus(
                "KQ ESP flashed successfully",
                true
            );


            // --------------------------------------
            // RESET
            // --------------------------------------

            log(
                "Resetting ESP32..."
            );

            setStage(
                "Resetting ESP32..."
            );


            try {

                await loader.after(
                    "hard_reset"
                );

                log(
                    "ESP32 reset complete."
                );

            } catch (resetError) {

                log(
                    `[WARNING] Reset failed: ${
                        getErrorMessage(
                            resetError
                        )
                    }`
                );

                log(
                    "Press the ESP32 RESET button manually."
                );
            }


            log("");
            log("================================");
            log("KQ ESP v0.4.2 READY");
            log("================================");


            setStage(
                "KQ ESP v0.4.2 ready"
            );


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

            setStage(
                "Flash failed"
            );

            setProgress(0);

        } finally {

            flashing = false;

            connectButton.disabled =
                connected;

            reconnectButton.disabled =
                false;

            flashButton.disabled =
                !connected;
        }
    }
);


// --------------------------------------------------
// CLEAR LOG
// --------------------------------------------------

clearLogButton.addEventListener(
    "click",
    () => {

        logElement.textContent =
            "KQ ESP Flasher v0.3\nConsole cleared.";
    }
);


// --------------------------------------------------
// BROWSER SERIAL EVENTS
// --------------------------------------------------

if ("serial" in navigator) {

    navigator.serial.addEventListener(
        "connect",
        (event) => {

            log(
                "USB serial device connected."
            );
        }
    );


    navigator.serial.addEventListener(
        "disconnect",
        async (event) => {

            if (
                event.port === port &&
                !flashing
            ) {

                log(
                    "USB serial device disconnected."
                );

                connected = false;

                flashButton.disabled =
                    true;

                connectButton.disabled =
                    false;

                reconnectButton.disabled =
                    false;

                connectButton.textContent =
                    "Select USB Device";

                setStatus(
                    "Device disconnected",
                    false
                );

                setPortInfo(
                    "Connect your ESP32 via USB"
                );

                setStage(
                    "Waiting for ESP32"
                );
            }
        }
    );
}


// --------------------------------------------------
// CLEANUP
// --------------------------------------------------

async function cleanupConnection() {

    try {

        if (transport) {
            await transport.disconnect();
        }

    } catch (_) {}

    port = null;
    transport = null;
    loader = null;
}


// --------------------------------------------------
// INITIAL STATE
// --------------------------------------------------

setStatus(
    "Waiting for device",
    false
);

setPortInfo(
    "Connect your ESP32 via USB"
);

setStage(
    "Waiting for ESP32"
);

setProgress(0);

if (!checkWebSerial()) {

    log(
        "Web Serial is unavailable in this browser."
    );

} else {

    log(
        "Connect your ESP32 via USB."
    );

    log(
        "Click 'Select USB Device' to choose the port."
    );
}

