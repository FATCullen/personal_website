const registry = [
  { name: '600 Cell',   load: () => import('./animations/600cell.js') },
  { name: 'Boids',      load: () => import('./animations/boids.js') },
  { name: 'Life',       load: () => import('./animations/life.js') },
  { name: 'N-Body',     load: () => import('./animations/nbody.js') },
  { name: 'Network',    load: () => import('./animations/net.js') },
  { name: 'Spacetime',  load: () => import('./animations/spacetime.js') },
  { name: 'Tesseract',  load: () => import('./animations/tesseract.js') },
];

let currentAnim = null;
let currentIndex = -1;
const colors = { bg: '', fg: '' };

function readColors() {
  const styles = getComputedStyle(document.documentElement);
  colors.bg = styles.getPropertyValue('--bg').trim();
  colors.fg = styles.getPropertyValue('--fg').trim();
}

async function loadAnimation(index) {
  if (currentAnim) currentAnim.stop();

  let canvas = document.querySelector('#bg-root canvas');
  if (!canvas) {
    canvas = document.createElement('canvas');
    document.getElementById('bg-root').appendChild(canvas);
  }

  const entry = registry[index];
  const { default: createAnimation } = await entry.load();
  currentAnim = createAnimation(canvas, colors);
  currentIndex = index;

  const nameEl = document.getElementById('bg-name');
  if (nameEl) nameEl.textContent = entry.name;

  const resize = () => currentAnim.resize(innerWidth, innerHeight);
  resize();
  addEventListener('resize', resize);
  currentAnim.start();
}

function pickIndex(excluding) {
  if (registry.length === 1) return 0;
  let i;
  do { i = Math.floor(Math.random() * registry.length); } while (i === excluding);
  return i;
}

function initTheme() {
  const stored = localStorage.getItem('theme');
  if (stored) document.documentElement.setAttribute('data-theme', stored);
  readColors();

  const toggle = document.getElementById('theme-toggle');
  if (!toggle) return;

  const isLight = () => getComputedStyle(document.documentElement)
    .getPropertyValue('--bg').trim() === '#ffffff';
  toggle.setAttribute('aria-pressed', String(isLight()));
  toggle.disabled = false;

  toggle.addEventListener('click', () => {
    const next = isLight() ? 'dark' : 'light';
    document.documentElement.setAttribute('data-theme', next);
    localStorage.setItem('theme', next);
    toggle.setAttribute('aria-pressed', String(next === 'light'));
    readColors();
  });
}

function initReroll() {
  const btn = document.getElementById('reroll-btn');
  if (!btn) return;
  btn.disabled = false;
  btn.addEventListener('click', () => loadAnimation(pickIndex(currentIndex)));
}

async function initBackground() {
  if (!document.getElementById('bg-root')) return;
  if (window.matchMedia('(prefers-reduced-motion: reduce)').matches) return;

  await loadAnimation(pickIndex(-1));
  document.addEventListener('visibilitychange', () => {
    if (!currentAnim) return;
    document.hidden ? currentAnim.stop() : currentAnim.start();
  });
}

function initBlogSearch() {
  const input = document.getElementById('blog-search');
  const items = document.querySelectorAll('.post-item');
  const empty = document.getElementById('blog-empty');
  if (!input || items.length === 0) return;

  input.addEventListener('input', () => {
    const q = input.value.trim().toLowerCase();
    let visibleCount = 0;

    items.forEach((item) => {
      const haystack = `${item.dataset.title} ${item.dataset.tags}`.toLowerCase();
      const match = haystack.includes(q);
      item.hidden = !match;
      if (match) visibleCount++;
    });

    if (empty) empty.hidden = visibleCount !== 0;
  });
}

initTheme();
initReroll();
initBackground();
initBlogSearch();