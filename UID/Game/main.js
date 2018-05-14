let lastClicked = null;
let cards = [];
let checkCards = [];
let nickName;
const numberOfPictures = 25;

onload = function () {
    showDialog();
};

// Choose your difficulty

function showDialog() {
    let dialog = document.getElementById('dialog');
    dialog.showModal();
    document.getElementById("startGame").onclick = function() {
        PlaySound("music/Cheeki Breeky.mp3");
        if (!(nickName = document.getElementById("nickName").value)){
            nickName = 'Anonymous';
        }
        dialog.close();
        document.getElementById("newGame").style.display = "flex";
        document.getElementById("timer").style.display = "flex";
        main();

        setTimer();
    };
}

function main(){
    let images = [];
    let image;
    let section = document.getElementById("gameSpace");
    for (let i=1; i <= getNumberOfParesCards(); i++){

	// Get unique card image

        do {
            image = "Images/" + (Math.floor(Math.random() * (numberOfPictures)) + 1).toString() + ".png";
        } while (images.indexOf(image) !== -1);
        images.push(image);

        let flip_container1 = makeFlipContainer(image);
        let flip_container2 = makeFlipContainer(image);
        flip_container1.setAttribute( "id", "myCard" + (2*i-1));
        flip_container2.setAttribute( "id", "myCard" + 2*i);

        cards.push(flip_container1);
        cards.push(flip_container2);
    }

    // shuffle curds
     cards = cards.sort(function(){
         return 0.5-Math.random()
     });

    cards = cards.forEach(function (elem) {
        section.appendChild(elem);
    });

    document.getElementById("newGame").onclick = function () {
        window.location.reload();
    };
}

// Making a card container

function makeFlipContainer(image){

    let front = document.createElement("div");
    front.classList.add("front");
    let back = document.createElement("div");
    back.classList.add("back");
    back.style.background = "#ffffff" + " url(" + image + ") no-repeat 50% 50%";
    back.style.backgroundSize = "contain";

    let flipper = document.createElement("div");
    flipper.classList.add("flipper");
    flipper.appendChild(front);
    flipper.appendChild(back);

    let flip_container = document.createElement("div");
    flip_container.classList.add("flip-container");

    flip_container.onclick = function(){
        if (checkCards.indexOf(this) === -1) {
            let elem = this;
            this.classList.toggle('flip');
            if (!lastClicked) {
                lastClicked = this;
            }
            else {
                if ((this.firstChild.lastChild.style.background === lastClicked.firstChild.lastChild.style.background) &&
                    (this.id !== lastClicked.id)) {
                    checkCards.push(this);
                    checkCards.push(lastClicked);
                    this.style.cursor = "default";
                    lastClicked.style.cursor = "default";
                    lastClicked = null;
                }
                else {
                    let temp = lastClicked;
                    lastClicked = null;
                    setTimeout(function () {
                        elem.classList.toggle('flip');
                        temp.classList.toggle('flip');
                        temp = null;
                    }, 1000);
                }
            }
        }
    };
    flip_container.appendChild(flipper);

    return flip_container;
}

// Timer

function setTimer() {
    let sec = 0;
    let elem = document.getElementById("timer");
    elem.innerHTML = "Match the countries! " + nickName + ": " +  sec;
    let timer = setInterval(function () {
        sec++;
        elem.innerHTML = "Match the countries! " +  nickName + ": " + sec;
        if (checkCards.length === getNumberOfParesCards()*2){
            clearInterval(timer);
            alert("Your time is " + sec);
        }
    }, 1000);
}

function getNumberOfParesCards() {
    let chooseLevel  = document.getElementById('chooseLevel');
    return chooseLevel[chooseLevel.selectedIndex].value;
}

function PlaySound(nameOfSong) {
    let audio = new Audio();
    audio.src = nameOfSong;
    audio.autoplay = true;
}
