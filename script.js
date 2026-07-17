let studentData = [];
let passFailChartInstance = null;
let marksChartInstance = null;

document.addEventListener('DOMContentLoaded', function() {
    document.documentElement.style.scrollBehavior = 'smooth';
    initHeaderActions();
    fetchStudents();
    
    // Auto-refresh the dashboard every 3 seconds!
    setInterval(fetchStudents, 3000);
});

function initHeaderActions() {
    const savedTheme = localStorage.getItem('theme') || 'light';
    document.documentElement.setAttribute('data-theme', savedTheme);

    const header = document.querySelector('header');
    const actionsDiv = document.createElement('div');
    actionsDiv.className = 'header-actions';
    
    const themeBtn = document.createElement('button');
    themeBtn.className = 'icon-btn';
    themeBtn.innerHTML = savedTheme === 'dark' ? '☀️' : '🌙';
    themeBtn.title = "Toggle Dark/Light Mode";
    themeBtn.addEventListener('click', () => {
        const currentTheme = document.documentElement.getAttribute('data-theme');
        const newTheme = currentTheme === 'light' ? 'dark' : 'light';
        document.documentElement.setAttribute('data-theme', newTheme);
        localStorage.setItem('theme', newTheme);
        themeBtn.innerHTML = newTheme === 'dark' ? '☀️' : '🌙';
        updateChartsTheme();
    });

    const printBtn = document.createElement('button');
    printBtn.className = 'icon-btn';
    printBtn.innerHTML = '🖨️';
    printBtn.title = 'Print Report';
    printBtn.onclick = () => window.print();

    const csvBtn = document.createElement('button');
    csvBtn.className = 'icon-btn';
    csvBtn.innerHTML = '📊';
    csvBtn.title = 'Download CSV Report';
    csvBtn.onclick = downloadCSV;

    actionsDiv.appendChild(themeBtn);
    actionsDiv.appendChild(printBtn);
    actionsDiv.appendChild(csvBtn);
    header.appendChild(actionsDiv);
}

function downloadCSV() {
    if (!studentData || studentData.length === 0) return;
    
    let csv = "Roll No,Student Name,Mother Name,Father Name,College,Course,Regulation,DOB,Percentage,Grade,Status\n";
    studentData.forEach(s => {
        csv += `"${s.rollNo}","${s.studentName}","${s.motherName}","${s.fatherName}","${s.collegeName}","${s.courseName}","${s.regulation}","${s.dob}","${s.result.percentage.toFixed(2)}","${s.result.grade}","${s.result.status}"\n`;
    });
    
    const blob = new Blob([csv], { type: 'text/csv;charset=utf-8;' });
    const link = document.createElement('a');
    const url = URL.createObjectURL(blob);
    link.setAttribute('href', url);
    link.setAttribute('download', 'student_report.csv');
    link.style.visibility = 'hidden';
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
}

let lastDataHash = "";
function fetchStudents() {
    fetch('/api/students')
        .then(response => response.text())
        .then(text => {
            if (text !== lastDataHash) {
                lastDataHash = text;
                studentData = JSON.parse(text);
                renderDashboard();
            }
        })
        .catch(err => {
            if (document.getElementById('studentsGrid').innerHTML.includes('Loading')) {
                document.getElementById('studentsGrid').innerHTML = `<h2 style="color:red; text-align:center;">Failed to connect to Live Server! Is it running?</h2>`;
            }
        });
}

function renderDashboard() {
    const grid = document.getElementById('studentsGrid');
    grid.innerHTML = '';

    // Remove old controls/stats if they exist to re-render clean
    document.querySelectorAll('.controls-container').forEach(e => e.remove());
    document.querySelectorAll('.dashboard-grid:not(#chartContainer)').forEach(e => e.remove());

    if (studentData.length === 0) {
        grid.innerHTML = `<h2 style="text-align:center;">No students found in the database!</h2>`;
        return;
    }

    addControls();
    addStatistics();
    renderCharts();
    
    // Render all student cards
    studentData.forEach((student, index) => {
        const cardHTML = generateStudentCard(student, index);
        grid.insertAdjacentHTML('beforeend', cardHTML);
    });

    initAnimations();
}

function generateStudentCard(student, index) {
    let subjectsHtml = '';
    student.subjects.forEach(sub => {
        subjectsHtml += `
            <tr>
                <td>${sub.name}</td>
                <td>${parseFloat(sub.marks).toFixed(2)}</td>
                <td>${sub.max}</td>
            </tr>
        `;
    });

    const maxTotal = student.subjects.length * 100;
    const statusClass = student.result.status === 'PASS' ? 'text-success' : 'text-danger';

    return `
        <article class="student-card glass" data-name="${student.studentName.toLowerCase()}" data-roll="${student.rollNo.toLowerCase()}" data-percentage="${student.result.percentage}" data-result="${student.result.status}" style="opacity:0; transform: translateY(40px);">
            <div class="card-header-info">
                <h3>${student.studentName}</h3>
                <div class="info-group">
                    <p><strong>Roll No:</strong> ${student.rollNo}</p>
                    <p><strong>Course:</strong> ${student.courseName}</p>
                    <p><strong>College:</strong> ${student.collegeName}</p>
                </div>
                <div class="info-group" style="text-align: right;">
                    <p><strong>Father's Name:</strong> ${student.fatherName}</p>
                    <p><strong>Mother's Name:</strong> ${student.motherName}</p>
                    <p><strong>DOB:</strong> ${student.dob}</p>
                    <p><strong>Regulation:</strong> ${student.regulation}</p>
                </div>
            </div>

            <div class="table-responsive">
                <table class="table">
                    <thead>
                        <tr><th>Subject</th><th>Marks Obtained</th><th>Max Marks</th></tr>
                    </thead>
                    <tbody>
                        ${subjectsHtml}
                    </tbody>
                    <tfoot>
                        <tr><td>Total</td><td>${student.result.total.toFixed(2)}</td><td>${maxTotal}</td></tr>
                    </tfoot>
                </table>
            </div>

            <div class="results-grid">
                <div class="result-box">
                    <h5>Percentage</h5>
                    <p class="text-info">${student.result.percentage.toFixed(2)}%</p>
                    <div class="progress-container">
                        <div class="progress-bar" data-width="${student.result.percentage}%" style="width: 0;"></div>
                    </div>
                </div>
                <div class="result-box">
                    <h5>Grade</h5>
                    <p class="text-warning">${student.result.grade}</p>
                </div>
                <div class="result-box">
                    <h5>Status</h5>
                    <p class="${statusClass}">${student.result.status}</p>
                </div>
            </div>

            <div class="signature-area" style="margin-top: 2.5rem; display: flex; justify-content: space-between; border-top: 2px dashed rgba(0,0,0,0.1); padding-top: 1.5rem; font-size: 1.1rem;">
                <div>
                    <strong>Dated:</strong> ${new Date().toLocaleDateString()}
                </div>
                <div>
                    <strong>Principal Signature:</strong> _______________________
                </div>
            </div>
        </article>
    `;
}

function addControls() {
    const header = document.querySelector('header');
    
    const controlsDiv = document.createElement('div');
    controlsDiv.className = 'controls-container glass';
    controlsDiv.style.marginBottom = '3.5rem';
    
    controlsDiv.innerHTML = `
        <div class="search-box">
            <input type="text" id="searchBox" placeholder="Search by student name or roll no...">
        </div>
        <div class="btn-group" role="group">
            <button type="button" class="btn active" data-filter="all">All</button>
            <button type="button" class="btn" data-filter="PASS">Pass</button>
            <button type="button" class="btn" data-filter="FAIL">Fail</button>
        </div>
        <div class="btn-group" role="group">
            <button type="button" class="btn" data-sort="percentage">Sort by %</button>
            <button type="button" class="btn" data-sort="name">Sort A-Z</button>
        </div>
    `;
    
    header.after(controlsDiv);
    
    document.getElementById('searchBox').addEventListener('input', (e) => {
        const term = e.target.value.toLowerCase();
        filterCards(term, document.querySelector('.btn-group .active').getAttribute('data-filter'));
    });
    
    const filterBtns = controlsDiv.querySelectorAll('[data-filter]');
    filterBtns.forEach(btn => {
        btn.addEventListener('click', (e) => {
            filterBtns.forEach(b => b.classList.remove('active'));
            e.target.classList.add('active');
            filterCards(document.getElementById('searchBox').value.toLowerCase(), e.target.getAttribute('data-filter'));
        });
    });
    
    controlsDiv.querySelectorAll('[data-sort]').forEach(btn => {
        btn.addEventListener('click', (e) => {
            sortCards(e.target.getAttribute('data-sort'));
        });
    });
}

function filterCards(searchTerm, filterStatus) {
    const cards = document.querySelectorAll('.student-card');
    cards.forEach(card => {
        const name = card.getAttribute('data-name');
        const roll = card.getAttribute('data-roll');
        const status = card.getAttribute('data-result');
        
        const matchesSearch = name.includes(searchTerm) || roll.includes(searchTerm);
        const matchesFilter = filterStatus === 'all' || status === filterStatus;
        
        if (matchesSearch && matchesFilter) {
            card.classList.remove('filtered-out');
        } else {
            card.classList.add('filtered-out');
        }
    });
}

function sortCards(criteria) {
    const grid = document.getElementById('studentsGrid');
    const cards = Array.from(grid.querySelectorAll('.student-card'));
    
    cards.sort((a, b) => {
        if (criteria === 'percentage') {
            const pctA = parseFloat(a.getAttribute('data-percentage'));
            const pctB = parseFloat(b.getAttribute('data-percentage'));
            return pctB - pctA; 
        } else {
            const nameA = a.getAttribute('data-name');
            const nameB = b.getAttribute('data-name');
            return nameA.localeCompare(nameB);
        }
    });
    
    cards.forEach(card => grid.appendChild(card));
}

function addStatistics() {
    const controls = document.querySelector('.controls-container');
    
    let pass = 0, fail = 0, totalPct = 0;
    
    studentData.forEach(student => {
        if (student.result.status === 'PASS') pass++;
        else fail++;
        totalPct += student.result.percentage;
    });
    
    const avg = studentData.length ? (totalPct / studentData.length).toFixed(1) : 0;
    
    const statsHTML = `
        <div class="dashboard-grid" style="margin-bottom: 2rem;">
            <div class="stat-card glass" style="border-bottom: 6px solid var(--primary-color)">
                <h5>Total Students</h5>
                <p class="display-4 count-up" data-target="${studentData.length}">0</p>
            </div>
            <div class="stat-card glass" style="border-bottom: 6px solid var(--success-color)">
                <h5>Passed</h5>
                <p class="display-4 count-up text-success" data-target="${pass}">0</p>
            </div>
            <div class="stat-card glass" style="border-bottom: 6px solid var(--danger-color)">
                <h5>Failed</h5>
                <p class="display-4 count-up text-danger" data-target="${fail}">0</p>
            </div>
            <div class="stat-card glass" style="border-bottom: 6px solid var(--info-color)">
                <h5>Average</h5>
                <p class="display-4"><span class="count-up" data-target="${avg}">0</span>%</p>
            </div>
        </div>
    `;
    
    controls.insertAdjacentHTML('afterend', statsHTML);
    animateNumbers();
}

function animateNumbers() {
    const counters = document.querySelectorAll('.count-up');
    const speed = 20;

    counters.forEach(counter => {
        counter.setAttribute('data-current', '0');
        const target = parseFloat(counter.getAttribute('data-target'));
        const inc = target / speed;

        const updateCount = () => {
            let current = parseFloat(counter.getAttribute('data-current'));
            if (current < target) {
                current += inc;
                if (current > target) current = target;
                counter.setAttribute('data-current', current);
                counter.innerText = current.toFixed(target % 1 !== 0 ? 1 : 0);
                setTimeout(updateCount, 30);
            } else {
                counter.innerText = target % 1 !== 0 ? target.toFixed(1) : target;
            }
        };
        updateCount();
    });
}

function initAnimations() {
    const observer = new IntersectionObserver((entries) => {
        entries.forEach((entry, index) => {
            if (entry.isIntersecting) {
                setTimeout(() => {
                    entry.target.style.opacity = '1';
                    entry.target.style.transform = 'translateY(0)';
                    const pb = entry.target.querySelector('.progress-bar');
                    if(pb) pb.style.width = pb.getAttribute('data-width');
                }, Math.min(index * 100, 500));
                observer.unobserve(entry.target);
            }
        });
    }, { threshold: 0.1 });

    document.querySelectorAll('.student-card').forEach(card => observer.observe(card));
}

// Chart.js Implementation
function renderCharts() {
    document.getElementById('chartContainer').style.display = 'grid';
    
    const isDark = document.documentElement.getAttribute('data-theme') === 'dark';
    const textColor = isDark ? '#f8fafc' : '#0f172a';

    Chart.defaults.color = textColor;
    Chart.defaults.font.family = 'Inter, sans-serif';

    // 1. Pass / Fail Pie Chart
    let pass = 0, fail = 0;
    studentData.forEach(s => s.result.status === 'PASS' ? pass++ : fail++);

    const ctx1 = document.getElementById('passFailChart').getContext('2d');
    if (passFailChartInstance) passFailChartInstance.destroy();
    passFailChartInstance = new Chart(ctx1, {
        type: 'doughnut',
        data: {
            labels: ['Pass', 'Fail'],
            datasets: [{
                data: [pass, fail],
                backgroundColor: ['#10b981', '#ef4444'],
                borderWidth: 0
            }]
        },
        options: { responsive: true, maintainAspectRatio: false }
    });

    // 2. Average Subject Marks Bar Chart
    const subjectsAgg = {};
    const subjectsCount = {};
    studentData.forEach(student => {
        student.subjects.forEach(sub => {
            if (!subjectsAgg[sub.name]) { subjectsAgg[sub.name] = 0; subjectsCount[sub.name] = 0; }
            subjectsAgg[sub.name] += sub.marks;
            subjectsCount[sub.name]++;
        });
    });

    const labels = Object.keys(subjectsAgg);
    const data = labels.map(l => (subjectsAgg[l] / subjectsCount[l]).toFixed(1));

    const ctx2 = document.getElementById('marksChart').getContext('2d');
    if (marksChartInstance) marksChartInstance.destroy();
    marksChartInstance = new Chart(ctx2, {
        type: 'bar',
        data: {
            labels: labels,
            datasets: [{
                label: 'Average Score',
                data: data,
                backgroundColor: '#3b82f6',
                borderRadius: 5
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            plugins: { legend: { display: false } },
            scales: { y: { beginAtZero: true, max: 100 } }
        }
    });
}

function updateChartsTheme() {
    if(!passFailChartInstance || !marksChartInstance) return;
    const isDark = document.documentElement.getAttribute('data-theme') === 'dark';
    const textColor = isDark ? '#f8fafc' : '#0f172a';
    
    Chart.defaults.color = textColor;
    passFailChartInstance.update();
    marksChartInstance.update();
}
