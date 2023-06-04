// Function to send an AJAX request to the server
function sendRequest(url, method, data, callback) {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (xhr.readyState === XMLHttpRequest.DONE) {
            if (xhr.status === 200) {
                var response = JSON.parse(xhr.responseText);
                callback(response);
            } else {
                console.error('Request failed. Status:', xhr.status);
            }
        }
    };
    xhr.open(method, url, true);
    xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    xhr.send(data);
}

// Function to display the student records table
function displayRecords(records) {
    var tableBody = document.querySelector('#records-table tbody');
    tableBody.innerHTML = '';

    records.forEach(function (record, index) {
        var row = tableBody.insertRow();
        row.insertCell().textContent = index + 1;
        row.insertCell().textContent = record.name;
        row.insertCell().textContent = record.marks;
    });
}

// Function to update the student select options
function updateStudentOptions(records) {
    var updateSelect = document.querySelector('#update-student');
    var deleteSelect = document.querySelector('#delete-student');

    updateSelect.innerHTML = '';
    deleteSelect.innerHTML = '';

    records.forEach(function (record, index) {
        var option = document.createElement('option');
        option.value = index + 1;
        option.textContent = record.name;
        updateSelect.appendChild(option);

        option = document.createElement('option');
        option.value = index + 1;
        option.textContent = record.name;
        deleteSelect.appendChild(option);
    });
}

// Function to handle the form submission for storing student marks
function handleStoreFormSubmit(event) {
    event.preventDefault();

    var nameInput = document.querySelector('#name');
    var marksInput = document.querySelector('#marks');

    var data = 'name=' + encodeURIComponent(nameInput.value) +
               '&marks=' + encodeURIComponent(marksInput.value);

    sendRequest('/store', 'POST', data, function (response) {
        if (response.success) {
            nameInput.value = '';
            marksInput.value = '';
            fetchData();
        } else {
            console.error('Error storing student marks:', response.message);
        }
    });
}

// Function to handle the form submission for updating student marks
function handleUpdateFormSubmit(event) {
    event.preventDefault();

    var studentSelect = document.querySelector('#update-student');
    var marksInput = document.querySelector('#update-marks');

    var data = 'studentNum=' + encodeURIComponent(studentSelect.value) +
               '&newMarks=' + encodeURIComponent(marksInput.value);

    sendRequest('/update', 'POST', data, function (response) {
        if (response.success) {
            marksInput.value = '';
            fetchData();
        } else {
            console.error('Error updating student marks:', response.message);
        }
    });
}

// Function to handle the form submission for deleting a student
function handleDeleteFormSubmit(event) {
    event.preventDefault();

    var studentSelect = document.querySelector('#delete-student');

    var data = 'studentNum=' + encodeURIComponent(studentSelect.value);

    sendRequest('/delete', 'POST', data, function (response) {
        if (response.success) {
            fetchData();
        } else {
            console.error('Error deleting student:', response.message);
        }
    });
}

// Function to fetch and display the student records
function fetchData() {
    sendRequest('/fetch', 'GET', null, function (response) {
        if (response.success) {
            displayRecords(response.records);
            updateStudentOptions(response.records);
        } else {
            console.error('Error fetching student records:', response.message);
        }
    });
}

// Attach event listeners to the forms
document.querySelector('#store-form').addEventListener('submit', handleStoreFormSubmit);
document.querySelector('#update-form').addEventListener('submit', handleUpdateFormSubmit);
document.querySelector('#delete-form').addEventListener('submit', handleDeleteFormSubmit);

// Fetch the initial data
fetchData();
