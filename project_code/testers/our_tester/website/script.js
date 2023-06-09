// Add dangerous and cool effects
const container = document.querySelector('.container');
const logo = document.querySelector('.logo');
const body = document.querySelector('body');

// Shake effect on the container
const shakeContainer = () => {
    container.style.transform = 'translateX(5px)';
    setTimeout(() => {
        container.style.transform = 'translateX(-5px)';
    }, 100);
    setTimeout(() => {
        container.style.transform = 'translateX(0)';
    }, 200);
};

// Randomly change the image source
const changeLogo = () => {
    const randomNumber = Math.floor(Math.random() * 5) + 1;
    logo.src = `https://m.media-amazon.com/images/I/714qbOjaPdL._AC_UF1000,1000_QL80_${randomNumber}.jpg`;
};

// Display surprise alerts
const displaySurpriseAlert = () => {
    const surprises = [
        "BOO! You've been pranked!",
        "Watch out for ghosts!",
        "Trick or treat!",
        "Prepare for a scare!",
        "Spooky surprises await you!",
    ];
    const randomIndex = Math.floor(Math.random() * surprises.length);
    alert(surprises[randomIndex]);
};

// Create and animate spiders in the background
const createSpiders = () => {
    const numSpiders = 5;
    for (let i = 0; i < numSpiders; i++) {
        const spider = document.createElement('div');
        spider.classList.add('spider');
        spider.style.top = `${Math.random() * 100}vh`;
        spider.style.left = `${Math.random() * 100}vw`;
        body.appendChild(spider);
    }
};

createSpiders();
