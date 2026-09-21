    
    const messages = [
      "Congratulations. You clicked a button.",
      "That was a terrible decision.",
      "The website is disappointed in you.",
      "You have unlocked absolutely nothing.",
      "🦆",
      "Why did you actually click it?",
      "System status: vibes.",
      "Error 404: purpose not found.",
      "You could be doing literally anything else."
    ];

    const emojis = ["💀", "🗿", "🔥", "🦆", "🍕", "🐸", "😭", "💩"];

    function doSomething() {
      document.getElementById("text").textContent =
        messages[Math.floor(Math.random() * messages.length)];

      document.body.style.background =
        `hsl(${Math.random() * 360}, 100%, 70%)`;

      const emoji = document.createElement("div");
      emoji.id = "emoji";
      emoji.textContent =
        emojis[Math.floor(Math.random() * emojis.length)];

      emoji.style.left = Math.random() * 100 + "vw";
      emoji.style.top = "80vh";

      document.body.appendChild(emoji);

      setTimeout(() => emoji.remove(), 2000);
    }