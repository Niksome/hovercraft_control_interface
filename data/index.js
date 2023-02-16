/*
This function is for reseting the steering so it is straght again (90°).
*/
function resetSteering() {
  // Reset slider
  slider = document.getElementById("pwmSlider3");
  slider.value = 90;

  // Reset Text
  document.getElementById("textslider_value_steering").innerHTML = slider.value;

  // Send reset value to server
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider_steering?value=" + slider.value, true);
  xhr.send();
}

/*
This function updates the value of a slider and sends it to the webserver, so the value of the motors/servo will be changed.
*/
function updateSliderPWM(element, param) {
  // Get the id of the slider element that triggered the event
  var slider_id = element.id;

  // Get the value of the slider using its id
  var slider_value = document.getElementById(slider_id).value;

  // Get the id of the text element that displays the value of the slider
  var text_id = "textslider_value_" + param;

  // Update the text element with the new slider value
  document.getElementById(text_id).innerHTML = slider_value;

  // Log the new slider value to the console
  console.log(text_id, slider_value);

  // Create a new XMLHttpRequest object to send the request to the server
  var xhr = new XMLHttpRequest();

  // Open the request to the server, using the GET method and passing the value of the slider as a query string
  xhr.open("GET", "/slider_" + param + "?value=" + slider_value, true);

  // Send the request to the server
  xhr.send();
}
