document.getElementById('sendQueryButton').addEventListener('click', function() {
    const queryInput = document.getElementById('queryInput').value;

    if (!queryInput) {
        alert("Please enter a query.");
        return;
    }

    sendQuery(queryInput);
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
    let previousResponses = [];

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

            // Update the UI only with the new responses
            const responseContainer = document.getElementById('queryResponses');
            newResponses.slice(previousResponses.length).forEach(response => {
                const para = document.createElement('p');
                para.innerText = response;
                responseContainer.appendChild(para);
            });

            // Update the previousResponses array to track already displayed responses
            previousResponses = newResponses;

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

