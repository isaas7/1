document.getElementById('sendQueryButton').addEventListener('click', function() {
    const queryInput = document.getElementById('queryInput');
    const queryText = queryInput.value.trim();

    if (!queryText) {
        alert("Please enter a query.");
        return;
    }

    // Display the sent message
    addMessage(queryText, 'right');

    // Clear the input box
    queryInput.value = '';

    // Send the query
    sendQuery(queryText);
});

function sendQuery(query) {
    fetch('/', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ message: query })
    })
    .then(response => response.json())
    .then(data => {
        if (data.query_id) {
            document.getElementById('queryStatus').innerText = "Query sent. Waiting for responses...";
            fetchQueryUpdates(data.query_id);
        } else {
            document.getElementById('queryStatus').innerText = "Error sending query.";
        }
    })
    .catch(error => {
        console.error('Error:', error);
        document.getElementById('queryStatus').innerText = "Error sending query.";
    });
}

function fetchQueryUpdates(queryId) {
    let previousResponsesLength = 0;
    let currentResponseText = '';
    let messageDiv;

    const interval = setInterval(() => {
        fetch(`/query_status/${queryId}`)
        .then(response => response.json())
        .then(data => {
            if (data.error) {
                document.getElementById('queryStatus').innerText = "Error: " + data.error;
                clearInterval(interval);
                return;
            }

            // Parse the status JSON string
            const status = JSON.parse(data.status);

            // Extract relevant fields
            const completed = status.completed;
            const newResponses = status.partial_responses;

            // Create a new message div on first response
            if (!messageDiv) {
                messageDiv = addMessage('', 'left');
            }

            // Append only the new words to the current response text
            newResponses.slice(previousResponsesLength).forEach(response => {
                currentResponseText += response + ' ';
            });

            // Update the text content of the existing message div
            messageDiv.innerText = currentResponseText.trim();

            // Update the previousResponsesLength to track the length of responses already processed
            previousResponsesLength = newResponses.length;

            // Stop fetching if the query is completed
            if (completed) {
                document.getElementById('queryStatus').innerText = "Query completed.";
                clearInterval(interval);
            }

        })
        .catch(error => {
            console.error('Error:', error);
            document.getElementById('queryStatus').innerText = "Error fetching query status.";
            clearInterval(interval);
        });
    }, 1000);
}

function addMessage(message, alignment) {
    const responseContainer = document.getElementById('queryResponses');
    const para = document.createElement('div');
    para.classList.add('response', alignment);

    const messageDiv = document.createElement('div');
    messageDiv.classList.add('response-text', alignment === 'right' ? 'right' : 'left');
    messageDiv.innerText = message;

    const timestamp = document.createElement('div');
    timestamp.classList.add('timestamp');
    timestamp.innerText = new Date().toLocaleTimeString();

    messageDiv.appendChild(timestamp);
    para.appendChild(messageDiv);
    responseContainer.appendChild(para);

    // Scroll to the bottom of the chat box
    responseContainer.scrollTop = responseContainer.scrollHeight;

    return messageDiv; // Return the message div for further updates
}

