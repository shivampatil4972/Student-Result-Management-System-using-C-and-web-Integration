document.addEventListener('DOMContentLoaded', function() {
    // Add smooth scroll behavior
    document.documentElement.style.scrollBehavior = 'smooth';
    
    // Add print button
    addPrintButton();
    
    // Add search functionality
    addSearchBar();
    
    // Add statistics dashboard
    addStatistics();
    
    // Add filter options
    addFilterOptions();
});

function addPrintButton() {
    const header = document.querySelector('header');
    const printBtn = document.createElement('button');
    printBtn.className = 'btn btn-light btn-lg mt-3';
    printBtn.innerHTML = '<i class="bi bi-printer"></i> Print Report';
    printBtn.onclick = () => window.print();
    header.appendChild(printBtn);
}

function addSearchBar() {
    const header = document.querySelector('header');
    const searchDiv = document.createElement('div');
    searchDiv.className = 'mt-3';
    searchDiv.innerHTML = `
        <input type="text" id="searchBox" class="form-control form-control-lg" 
               placeholder="Search by student name or roll number..." 
               style="border-radius: 25px; padding: 15px 25px;">
    `;
    header.appendChild(searchDiv);
    
    document.getElementById('searchBox').addEventListener('input', function(e) {
        const searchTerm = e.target.value.toLowerCase();
        const studentCards = document.querySelectorAll('.student-card');
        
        studentCards.forEach(card => {
            const text = card.textContent.toLowerCase();
            card.style.display = text.includes(searchTerm) ? 'block' : 'none';
        });
    });
}

function addStatistics() {
    const container = document.querySelector('.container-fluid');
    const studentCards = document.querySelectorAll('.student-card');
    
    if (studentCards.length === 0) return;
    
    let totalStudents = studentCards.length;
    let passCount = 0;
    let failCount = 0;
    let totalPercentage = 0;
    let gradeDistribution = { 'A+': 0, 'A': 0, 'B': 0, 'C': 0, 'D': 0, 'F': 0 };
    
    studentCards.forEach(card => {
        const resultText = card.querySelector('.text-success, .text-danger');
        if (resultText) {
            if (resultText.textContent.includes('PASS')) passCount++;
            else failCount++;
        }
        
        const percentageText = card.querySelector('.text-info');
        if (percentageText) {
            totalPercentage += parseFloat(percentageText.textContent);
        }
        
        const gradeText = card.querySelector('.text-warning');
        if (gradeText && gradeDistribution[gradeText.textContent]) {
            gradeDistribution[gradeText.textContent]++;
        }
    });
    
    const avgPercentage = (totalPercentage / totalStudents).toFixed(2);
    
    const statsHTML = `
        <div class="statistics-dashboard mb-5 p-4 shadow-lg" style="background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); border-radius: 15px; color: white;">
            <h2 class="text-center mb-4">Performance Dashboard</h2>
            <div class="row g-4">
                <div class="col-md-3">
                    <div class="stat-card p-3 text-center" style="background: rgba(255,255,255,0.2); border-radius: 10px;">
                        <h5>Total Students</h5>
                        <p class="display-4">${totalStudents}</p>
                    </div>
                </div>
                <div class="col-md-3">
                    <div class="stat-card p-3 text-center" style="background: rgba(40,167,69,0.3); border-radius: 10px;">
                        <h5>Pass</h5>
                        <p class="display-4">${passCount}</p>
                    </div>
                </div>
                <div class="col-md-3">
                    <div class="stat-card p-3 text-center" style="background: rgba(220,53,69,0.3); border-radius: 10px;">
                        <h5>Fail</h5>
                        <p class="display-4">${failCount}</p>
                    </div>
                </div>
                <div class="col-md-3">
                    <div class="stat-card p-3 text-center" style="background: rgba(23,162,184,0.3); border-radius: 10px;">
                        <h5>Avg %</h5>
                        <p class="display-4">${avgPercentage}%</p>
                    </div>
                </div>
            </div>
            <div class="mt-4">
                <h4 class="text-center mb-3">Grade Distribution</h4>
                <div class="row g-2">
                    ${Object.entries(gradeDistribution).map(([grade, count]) => `
                        <div class="col-2">
                            <div class="text-center p-2" style="background: rgba(255,255,255,0.15); border-radius: 8px;">
                                <strong>${grade}</strong><br>${count}
                            </div>
                        </div>
                    `).join('')}
                </div>
            </div>
        </div>
    `;
    
    container.insertAdjacentHTML('afterbegin', statsHTML);
}

function addFilterOptions() {
    const header = document.querySelector('header');
    const filterDiv = document.createElement('div');
    filterDiv.className = 'mt-3';
    filterDiv.innerHTML = `
        <div class="btn-group" role="group">
            <button type="button" class="btn btn-light" onclick="filterBy('all')">All</button>
            <button type="button" class="btn btn-light" onclick="filterBy('pass')">Pass</button>
            <button type="button" class="btn btn-light" onclick="filterBy('fail')">Fail</button>
            <button type="button" class="btn btn-light" onclick="sortBy('percentage')">Sort by %</button>
            <button type="button" class="btn btn-light" onclick="sortBy('name')">Sort by Name</button>
        </div>
    `;
    header.appendChild(filterDiv);
}

function filterBy(criteria) {
    const studentCards = document.querySelectorAll('.student-card');
    
    studentCards.forEach(card => {
        if (criteria === 'all') {
            card.style.display = 'block';
        } else if (criteria === 'pass') {
            const hasPass = card.querySelector('.text-success');
            card.style.display = hasPass ? 'block' : 'none';
        } else if (criteria === 'fail') {
            const hasFail = card.querySelector('.text-danger');
            card.style.display = hasFail ? 'block' : 'none';
        }
    });
}

function sortBy(criteria) {
    const container = document.querySelector('.container-fluid');
    const studentCards = Array.from(document.querySelectorAll('.student-card'));
    
    studentCards.sort((a, b) => {
        if (criteria === 'percentage') {
            const percentA = parseFloat(a.querySelector('.text-info').textContent);
            const percentB = parseFloat(b.querySelector('.text-info').textContent);
            return percentB - percentA;
        } else if (criteria === 'name') {
            const nameA = a.querySelector('h3').textContent;
            const nameB = b.querySelector('h3').textContent;
            return nameA.localeCompare(nameB);
        }
    });
    
    const statsDiv = container.querySelector('.statistics-dashboard');
    studentCards.forEach(card => {
        container.appendChild(card);
    });
    
    if (statsDiv) {
        container.insertBefore(statsDiv, container.firstChild.nextSibling);
    }
}

// Add animation on scroll
const observer = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
        if (entry.isIntersecting) {
            entry.target.style.opacity = '1';
            entry.target.style.transform = 'translateY(0)';
        }
    });
}, { threshold: 0.1 });

document.querySelectorAll('.student-card').forEach(card => {
    card.style.opacity = '0';
    card.style.transform = 'translateY(20px)';
    card.style.transition = 'all 0.6s ease-out';
    observer.observe(card);
});
