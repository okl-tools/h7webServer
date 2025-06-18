#include "page.h"
#include <stdio.h>
#include <stdint.h>


#include "PFunction.h"
#include "PTools.h"
#include "PWriter.h"
#include "PStack.h"
#include "json/Node.h"
#include "somePages.h"

extern "C" {
#include "main.h" // Für C-Funktionen wie HAL_Delay
void http_write (struct netconn * conn, const char * data, size_t len);
}

using namespace ptools;
using namespace ptools::json;


const char * page_main()
{
    return R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>NUCLEO-H723ZG</title>
    <style>
        body
        {
            margin: 0;
            font-family: Arial, sans-serif;
            background-color: #001f3f;
            color: white;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: flex-start;
            height: 100vh;
            padding: 20px;
            box-sizing: border-box;
        }

        h1, h2
        {
            margin: 10px;
            text-align: center;
        }

        .button-container
        {
            display: flex;
            flex-direction: column;
            width: 100%;
            max-width: 400px;
            gap: 20px;
            margin-top: 40px;
        }

        .nav-button
        {
            background-color: #0074D9;
            color: white;
            border: none;
            padding: 20px;
            font-size: 1.5rem;
            border-radius: 10px;
            text-align: center;
            text-decoration: none;
            transition: background-color 0.3s ease;
        }

        .nav-button:hover
        {
            background-color: #005fa3;
        }

        @media (min-width: 600px)
        {
            .button-container
            {
                flex-direction: row;
                justify-content: center;
            }
        }
    </style>
</head>
<body>
    <h1>NUCLEO-H723ZG</h1>
    <h2>FreeRTOS™ + lwIP</h2>

    <div class="button-container">
        <a class="nav-button" href="/blinky">LED Control</a>
        <a class="nav-button" href="/STM32H7xx.html">Original STM32H7xx Webserver Demo</a>
        <a class="nav-button" href="/STM32H7xxTASKS.html">Tasks</a>
    </div>
</body>
</html>
    )";
}

const char * page_main_old ()
{
    return R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>NUCLEO-H723ZG</title>
    <style>
        body {
            font-family: sans-serif;
            margin: 0;
            padding: 0;
            background: #053798;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
        }

        h1 {
            font-size: 2rem;
            text-align: center;
            color: white;
            width: 100%;
        }

        h2 {
            font-size: 1.5em;
            text-align: center;
            color: white;
            width: 100%;
        }


        .slider-container {
            width: 90%;
            max-width: 600px;
            margin: 1rem 0;
            background: white;
            padding: 1rem;
            border-radius: 8px;
            box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
        }

        .info {
            color: white;
            width: 100%;
            text-align: center;
        }

        label {
            display: block;
            margin-top: 1.5rem;
            font-weight: bold;
        }

        input[type="range"] {
            width: 100%;
        }

        .value-display {
            text-align: right;
            font-size: 0.9rem;
            color: #555;
        }


        @media (orientation: landscape) {
            body {
                flex-direction: row;
                flex-wrap: wrap;
                justify-content: center;
                align-items: flex-start;
                padding: 2rem;
            }

            .slider-container {
                width: 45%;
                margin: 1rem;
            }
        }
    </style>
</head>
<body>
<h1>NUCLEO-H723ZG</h1>
<h2>FreeRTOS™ + lwIP</h2>

<div class="slider-container">
<p>Blinky</p>
<p>STM 32 Original</p>
<p>Bla</p>
</div>

<div class="info">
    Communication via json over http </br>
    Realized with <b>ptools (C)</b> by Olaf Kliche
</div>

</body>
</html>
    )";
}

const char * page_blink_control ()
{
    return R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>NUCLEO-H723ZG</title>
    <style>
        body {
            font-family: sans-serif;
            margin: 0;
            padding: 0;
            background-color: #001f3f;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
        }

        h1 {
            font-size: 2rem;
            text-align: center;
            color: white;
            width: 100%;
        }
        a {
            color: white;
        }

        h2 {
            font-size: 1.5em;
            text-align: center;
            color: white;
            width: 100%;
        }


        .slider-container {
            width: 90%;
            max-width: 600px;
            margin: 1rem 0;
            background: white;
            padding: 1rem;
            border-radius: 8px;
            box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
        }

        .info {
            color: white;
            width: 100%;
            text-align: center;
        }

        label {
            display: block;
            margin-top: 1.5rem;
            font-weight: bold;
        }

        input[type="range"] {
            width: 100%;
        }

        .value-display {
            text-align: right;
            font-size: 0.9rem;
            color: #555;
        }


        @media (orientation: landscape) {
            body {
                flex-direction: row;
                flex-wrap: wrap;
                justify-content: center;
                align-items: flex-start;
                padding: 2rem;
            }

            .slider-container {
                width: 45%;
                margin: 1rem;
            }
        }
    </style>
</head>
<body>
<h1>NUCLEO-H723ZG</h1>
<h2>FreeRTOS™ + lwIP</h2>
<h2>'blinky' LED Control</h2>

<div class="slider-container">
    <label for="ledYellow">LED-yellow (Milliseconds)</label>
    <input type="range" id="ledYellow" min="30" max="3000" value="500">
    <div class="value-display" id="valYellow">500 ms</div>

    <label for="ledRed">LED-red (Milliseconds)</label>
    <input type="range" id="ledRed" min="30" max="3000" value="500">
    <div class="value-display" id="valRed">500 ms</div>
</div>

<div class="info">
    Communication via json over http </br>
    Realized with <b><a href="https://okl.de" target="pt">ptools</a></b> by <a href="https://okl.de" target="okl">Olaf Kliche</a>
</div>

<script>
    const yellowSlider = document.getElementById("ledYellow");
    const redSlider = document.getElementById("ledRed");

    const valYellow = document.getElementById("valYellow");
    const valRed = document.getElementById("valRed");

    function sendJSON()
    {
        const data = {
            led_yellow: parseInt(yellowSlider.value),
            led_red: parseInt(redSlider.value)
        };

        // Update display
        valYellow.textContent = data.led_yellow + " ms";
        valRed.textContent = data.led_red + " ms";

        // Send POST request
        fetch("/api/led", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(data)
        }).catch(err => console.warn("POST failed", err));
    }

    yellowSlider.addEventListener("input", sendJSON);
    redSlider.addEventListener("input", sendJSON);

    // Initial send
    sendJSON();

    // Optional: React to orientation change
    window.addEventListener("orientationchange", () =>
    {
        console.log("Orientation changed:", screen.orientation?.type || "unknown");
    });
</script>
</body>
</html>
    )";
}


const char * page_person ()
{
    return R"(
<!DOCTYPE html>
<html lang="de">
<head>
  <meta charset="UTF-8">
  <title>Person erstellen</title>
</head>
<body>

  <h2>Person anlegen</h2>

  <form id="myForm">
    <label>Name: <input name="name" required></label><br>
    <label>Alter: <input name="age" type="number" required></label><br>
    <label>Hobby: <input name="hobby"></label><br><br>
    <button type="submit">[Create]</button>
  </form>

  <div id="response" style="margin-top:1em; color:green;"></div>

  <script>
    myForm.onsubmit = async e =>
    {
      e.preventDefault();

      const res = await fetch("/api/person", {
        method: "POST",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify(Object.fromEntries(new FormData(myForm)))
      });

      const text = await res.text();

      const r = document.getElementById("response");
      r.innerText = text;

      // Nachricht anzeigen + nach 5 Sekunden zur Startseite weiterleiten
      setTimeout(() => window.location.href = "/", 5000);
    };
  </script>

</body>
</html>
    )";
}

