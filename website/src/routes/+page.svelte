<script>
  import { onMount } from 'svelte';
  let ScrollY = 0;

  const HandleScroll = () => {
    ScrollY = window.scrollY;
  };

  onMount(() => {
    window.addEventListener('scroll', HandleScroll);
    return () => window.removeEventListener('scroll', HandleScroll);
  });

  const ScrollToInfo = () => {
    const el = document.getElementById("info");
    if (el) {
      el.scrollIntoView({ behavior: "smooth" });
    }
  };
</script>

<style>
  :global(body) {
    overflow-x: hidden;
    margin: 0;
    font-family: 'Segoe UI', sans-serif;
    scroll-behavior: smooth;
    background: #f9f9f9;
  }

  .hero {
    position: relative;
    height: 100vh;
    overflow: hidden;
  }

  .parallax-bg {
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background-image: url('/image.png');
    background-size: cover;
    background-position: center;
    z-index: 0;
    will-change: transform;
  }

  .overlay-gradient {
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background: linear-gradient(to bottom, rgba(0, 0, 0, 0.4), rgba(0, 0, 0, 0.75));
    z-index: 1;
  }

  .hero-content {
    position: relative;
    z-index: 2;
    height: 100%;
    display: flex;
    align-items: center;
    justify-content: center;
    color: white;
    text-align: center;
    padding: 2rem;
    opacity: 0;
    animation: fadeIn 1.2s ease-out forwards;
    animation-delay: 0.3s;
  }

  .hero-content h1 {
    font-size: 4rem;
    margin-bottom: 1.5rem;
    text-shadow: 0 0 10px rgba(0, 0, 0, 0.6);
  }

  .buttons {
    display: flex;
    gap: 1rem;
    justify-content: center;
    flex-wrap: wrap;
  }

  .btn {
    text-decoration: none;
    padding: 0.8rem 1.6rem;
    border-radius: 8px;
    font-size: 1.1rem;
    transition: all 0.25s ease-in-out;
    box-shadow: 0 4px 12px rgba(0, 0, 0, 0.2);
  }

  .btn.primary {
    background: #0d6efd;
    color: white;
  }

  .btn.primary:hover {
    background: #0b5ed7;
  }

  .btn.secondary {
    background: rgba(255, 255, 255, 0.15);
    color: white;
    border: 1px solid rgba(255, 255, 255, 0.3);
  }

  .btn.secondary:hover {
    background: rgba(255, 255, 255, 0.25);
  }

  @keyframes fadeIn {
    to {
      opacity: 1;
    }
  }

  .content {
    width: 100%;
    max-width: none;
    margin: 0;
    padding: 4rem 2rem;
    display: flex;
    flex-wrap: wrap;
    gap: 3rem;
    background: white;
    border-top: 2px solid #eee;
    animation: slideIn 1s ease forwards;
  }

  .content .text,
  .content .image {
    flex: 1;
    min-width: 0;
  }

  .content .image img {
    width: 100%;
    border-radius: 20px;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    max-height: 800px;
    object-fit: cover;
    margin-top: 200px;
  }

  @keyframes slideIn {
    from {
      opacity: 0;
      transform: translateY(30px);
    }
    to {
      opacity: 1;
      transform: translateY(0);
    }
  }

  h2 {
    margin-top: 2rem;
    font-size: 2rem;
    color: #222;
  }

  ul, ol {
    padding-left: 1.5rem;
  }

  li {
    margin-bottom: 0.5rem;
  }

  p {
    margin-top: 0.5rem;
  }

  code {
    background: #eee;
    padding: 0.2rem 0.4rem;
    border-radius: 4px;
    font-family: monospace;
  }

  @media (max-width: 768px) {
    .hero-content h1 {
      font-size: 3rem;
    }

    .content {
      flex-direction: column;
      padding: 2rem 1rem;
    }
  }
</style>

<!-- Hero Section with Parallax Background -->
<div class="hero">
  <div
    class="parallax-bg"
    style="transform: translateY({ScrollY * 0.5}px);"
  ></div>
  <div class="overlay-gradient"></div>
  <div class="hero-content">
    <div>
      <h1>R6 No Recoil</h1>
      <p style="font-size: 1.25rem; margin-top: -1rem; margin-bottom: 1.5rem; max-width: 650px; margin-left: auto; margin-right: auto; line-height: 1.6;">
        Open-source, fast, and lightweight no-recoil tool for Rainbow Six Siege. Easy to use and optimised for smooth performance.
      </p>
      <div class="buttons">
        <a
          href="https://github.com/Harry-Hopkinson/R6-No-Recoil/releases"
          target="_blank"
          class="btn primary"
        >⬇ Download</a>
        <button class="btn secondary" on:click={ScrollToInfo}>
          📖 Learn More
        </button>
      </div>
    </div>
  </div>
</div>

<!-- Main Info Section -->
<div id="info" class="content">
  <!-- Left: Image -->
  <div class="image">
    <img src="/app.png" alt="App preview" />
  </div>

  <!-- Right: Info Content -->
  <div class="text">
    <h2>🎯 What is R6 No Recoil?</h2>
    <p>A lightweight, efficient no-recoil tool for Rainbow Six Siege. Designed for smooth performance and ease of use.</p>

    <h2>🚀 Why Use This Tool?</h2>
    <ul>
      <li>📌 Reduces weapon recoil for better accuracy</li>
      <li>⚡ Lightweight & fast — optimised for low CPU usage</li>
      <li>🔧 No installation required — just run the <code>.exe</code></li>
      <li>🖥️ Works on Windows 10 & 11</li>
      <li>🛠️ Open source & customisable</li>
    </ul>

    <h2>🎮 How to Use</h2>
    <ol>
      <li>Download <code>r6-no-recoil.exe</code> from GitHub</li>
      <li>Double-click to run the program</li>
      <li><strong>Press Enter</strong> to toggle the no-recoil effect</li>
      <li><strong>Scroll mouse</strong> to adjust speed</li>
    </ol>

    <h2>🚧 Currently in Development</h2>
    <ul>
      <li>🔁 Crouch macro feature</li>
      <li>🎨 Improved GUI and UX</li>
      <li>⚙ Advanced customisation options</li>
    </ul>

    <h2>⚠️ Disclaimer</h2>
    <p>This tool is for <strong>educational purposes only</strong>. Windows Defender may show a false positive — check using VirusTotal if unsure.</p>
  </div>
</div>
