const firebaseConfig = {
    apiKey: "AIzaSyDBtZrYcAbGwSobGZH1CnJp0ATmT1JXdaM",
    authDomain: "doan1-c6c5e.firebaseapp.com",
    databaseURL: "https://doan1-c6c5e-default-rtdb.firebaseio.com",
    projectId: "doan1-c6c5e",
    storageBucket: "doan1-c6c5e.appspot.com",
    messagingSenderId: "930759091083",
    appId: "1:930759091083:web:f7b68d9a6f7dc7e008458e"
};
firebase.initializeApp(firebaseConfig);

$("#tempp").roundSlider({
    circleShape: "half",
    min: 0,
    max: 50,
    radius: 125,
    width: 20,
    handleSize: "+10",
    handleShape: "dot",
    sliderType: "min-range",
    readOnly: true,
});
firebase.database().ref("/The Weather/Temp").on("value", function(snapshot) {
    var nd = snapshot.val();
    $("#tempp").roundSlider("setValue", nd);
});
$("#hum").roundSlider({
    circleShape: "half",
    min: 0,
    max: 100,
    radius: 125,
    width: 20,
    handleSize: "+10",
    handleShape: "dot",
    sliderType: "min-range",
    readOnly: true,
});
firebase.database().ref("/The Weather/Humi").on("value", function(snapshot) {
    var nd = snapshot.val();
    $("#hum").roundSlider("setValue", nd);
});
$("#dad").roundSlider({
    circleShape: "half",
    min: 0,
    max: 100,
    radius: 125,
    width: 20,
    handleSize: "+10",
    handleShape: "dot",
    sliderType: "min-range",
    readOnly: true,
});
firebase.database().ref("/The Weather/Soil").on("value", function(snapshot) {
    var nd = snapshot.val();
    $("#dad").roundSlider("setValue", nd);
});
firebase.database().ref("/The Weather/Canh bao nhiet do").on("value", function(snapshot) {
    var motionValue = snapshot.val();
    document.getElementById("Checkbox").checked = motionValue;
});
window.onload = function() {
    var database = firebase.database();
    var temperatureSliderValueRef = database.ref('/The Weather/DKND');
    temperatureSliderValueRef.once('value').then(function(snapshot) {
        var temperatureValue = snapshot.val();
        var temperatureSlider = document.getElementById('nd');
        var temperatureRangeValue = document.getElementById('rangeValuee');

        temperatureSlider.value = temperatureValue;
        temperatureRangeValue.textContent = temperatureValue + '°C';
    });
    var moistureSliderValueRef = database.ref('/The Weather/DA');
    moistureSliderValueRef.once('value').then(function(snapshot) {
        var moistureValue = snapshot.val();
        var moistureSlider = document.getElementById('da');
        var moistureRangeValue = document.getElementById('rangeValue');
        moistureSlider.value = moistureValue;
        moistureRangeValue.textContent = moistureValue + '%';
    });
};

function updateTemperatureValue(value) {
    document.getElementById('rangeValuee').textContent = value + '°C';
}

function updateMoistureValue(value) {
    document.getElementById('rangeValue').textContent = value + '%';
}

function updateValue(value) {
    document.getElementById('rangeValuee').textContent = value + '°C';
}

var sliderr = document.getElementById('nd');
var rangeValuee = document.getElementById('rangeValuee');
document.getElementById("nd").addEventListener("input", function() {
    var ndValue = this.value;
    rangeValuee.textContent = ndValue + '°C';
    firebase.database().ref("/The Weather/DKND").set(ndValue);
});
rangeValuee.textContent = sliderr.value;

var slider = document.getElementById('da');
var rangeValue = document.getElementById('rangeValue');
document.getElementById("da").addEventListener("input", function() {
    var daValue = this.value;
    rangeValue.textContent = daValue + '%';
    firebase.database().ref("/The Weather/DA").set(daValue);
});
rangeValue.textContent = slider.value;


//Chế độ auto

firebase.database().ref("/The Weather/Auto").on("value", function(snapshot) {
    var nd = snapshot.val();
    if (nd == 1)
        document.getElementById("auto_img").src = "./img/auto_on.png"
    else
        document.getElementById("auto_img").src = "./img/auto_off.png"
});


//Bơm
firebase.database().ref("/The Weather/Pump").on("value", function(snapshot) {
    var p = snapshot.val();
    if (p == 1)
        document.getElementById("Pum_img").src = "./img/pump_on.png"
    else
        document.getElementById("Pum_img").src = "./img/pump_off.png"
});

//Đèn sưởi
firebase.database().ref("/The Weather/Den Suoi").on("value", function(snapshot) {
    var d = snapshot.val();
    if (d == 1)
        document.getElementById("Suoi_img").src = "./img/light_on.png"
    else
        document.getElementById("Suoi_img").src = "./img/light_off.png"
});