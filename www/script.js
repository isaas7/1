let lastResponse = ""; // Variable to store the last response from the LLM
const ctx = document.getElementById('performanceChart').getContext('2d');
let chart; // Reference to the Chart.js instance

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

    // Send the query with the previous response as context
    sendQuery(queryText, lastResponse);
});

function sendQuery(query) {
    fetch('/', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({
            message: query,
            context: {
                response: lastResponse, // Include the last response as context
                done: true
            }
        })
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

                // Store the last response in the global variable for use in the next query
                lastResponse = currentResponseText.trim();
            }

        })
        .catch(error => {
            console.error('Error:', error);
            document.getElementById('queryStatus').innerText = "Error fetching query status.";
            clearInterval(interval);
        });
    }, 1000);
}

function fetchPerformanceStatistics() {
    fetch('/performance_statistics')
        .then(response => response.json())
        .then(data => {
            const currentTime = new Date().toLocaleTimeString(); // Get the current time

            if (chart) {
                // Update the existing chart with new data
                chart.data.labels.push(currentTime);

                chart.data.datasets[0].data.push(data[0].average_value);
                chart.data.datasets[1].data.push(data[0].count);
                chart.data.datasets[2].data.push(data[0].max_value);
                chart.data.datasets[3].data.push(data[0].min_value);
                chart.data.datasets[4].data.push(data[0].total_value);

                // Keep the last 10 entries in the chart
                if (chart.data.labels.length > 10) {
                    chart.data.labels.shift();
                    chart.data.datasets.forEach(dataset => dataset.data.shift());
                }

                chart.update();
            } else {
                // Create the chart if it doesn't exist
                chart = new Chart(ctx, {
                    type: 'line',
                    data: {
                        labels: [currentTime],
                        datasets: [
                            {
                                label: 'Average',
                                data: [data[0].average_value],
                                borderColor: 'rgba(75, 192, 192, 1)',
                                fill: false
                            },
                            {
                                label: 'Count',
                                data: [data[0].count],
                                borderColor: 'rgba(54, 162, 235, 1)',
                                fill: false
                            },
                            {
                                label: 'Max',
                                data: [data[0].max_value],
                                borderColor: 'rgba(255, 99, 132, 1)',
                                fill: false
                            },
                            {
                                label: 'Min',
                                data: [data[0].min_value],
                                borderColor: 'rgba(255, 206, 86, 1)',
                                fill: false
                            },
                            {
                                label: 'Total',
                                data: [data[0].total_value],
                                borderColor: 'rgba(153, 102, 255, 1)',
                                fill: false
                            }
                        ]
                    },
                    options: {
                        scales: {
                            x: {
                                title: {
                                    display: true,
                                    text: 'Time'
                                }
                            },
                            y: {
                                title: {
                                    display: true,
                                    text: 'Value'
                                }
                            }
                        },
                        plugins: {
                            legend: {
                                display: true,
                                position: 'top'
                            }
                        }
                    }
                });
            }
        })
        .catch(error => {
            console.error('Error fetching performance statistics:', error);
        });
}

// Fetch statistics on an interval
setInterval(fetchPerformanceStatistics, 1000); // Fetch every 5 seconds

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

