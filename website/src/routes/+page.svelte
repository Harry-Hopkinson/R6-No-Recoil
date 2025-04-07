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

  .hero-content {
    position: relative;
    z-index: 1;
    height: 100%;
    display: flex;
    align-items: center;
    justify-content: center;
    color: white;
    font-size: 4rem;
    text-align: center;
    padding: 2rem;
    text-shadow: 0 0 10px rgba(0, 0, 0, 0.6);
    opacity: 0;
    animation: fadeIn 1.2s ease-out forwards;
    animation-delay: 0.3s;
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
    .hero-content {
      font-size: 2.5rem;
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
  <div class="hero-content">
    R6 No Recoil
  </div>
</div>

<!-- Main Content -->
<div class="content">
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
