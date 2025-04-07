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
    background-image: url('/Image.png');
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
    box-shadow: 0 4px 12px rgba(0,0,0,0.2);
  }

  .btn.primary {
    background: #0d6efd;
    color: white;
  }

  .btn.primary:hover {
    background: #0b5ed7;
  }

  .btn.secondary {
    background: rgba(255,255,255,0.15);
    color: white;
    border: 1px solid rgba(255,255,255,0.3);
  }

  .btn.secondary:hover {
    background: rgba(255,255,255,0.25);
  }

  @keyframes fadeIn {
    to {
      opacity: 1;
    }
  }

  .content {
    max-width: 800px;
    margin: 0 auto;
    padding: 4rem 2rem;
    background: white;
    border-top: 2px solid #eee;
    animation: slideIn 1s ease forwards;
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

  @media (max-width: 600px) {
    .hero-content h1 {
      font-size: 3rem;
    }

    .content {
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

<!-- Main Content -->
<div id="info" class="content">
  <h2>🛠 Features</h2>
  <ul>
    <li>🎯 Real-time recoil compensation</li>
    <li>⚙ Customizable presets</li>
    <li>🌓 Dark/Light theme toggle</li>
    <li>💾 Config file support (TOML)</li>
  </ul>

  <h2>📦 Lightweight</h2>
  <p>The latest build is just 200 KB — down from 1.2 MB — thanks to a custom lightweight configuration parser.</p>

  <h2>🚀 How to Use</h2>
  <ol>
    <li>Launch the app</li>
    <li>Choose your recoil preset</li>
    <li>Toggle on/off using the UI or Caps Lock</li>
  </ol>

  <h2>🔒 Disclaimer</h2>
  <p>This tool is designed for educational purposes and offline/bot matches. Use at your own risk.</p>
</div>
